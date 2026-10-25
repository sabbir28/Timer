#include "app/controller/app_controller.h"
#include "platform/windows/win32_hotkey.h"
#include "platform/windows/win32_power.h"
#include "services/audio/audio_service.h"
#include "services/notification/toast_service.h"
#include "services/persistence/settings_persistence.h"
#include "services/net/time_sync.h"
#include "services/color/color_adapter.h"
#include "services/logging/logger.h"
#include "ui/animation/animator.h"
#include "ui/renderers/renderer.h"
#include <commdlg.h>
#include <stdio.h>
#include <time.h>

static AppController g_app = {0};
static COLORREF g_custColors[16] = {0};

AppController* app_controller_get_instance(void)
{
    return &g_app;
}

static void format_time_str(uint32_t total_sec, wchar_t* buf, size_t buflen)
{
    uint32_t h = total_sec / 3600;
    uint32_t m = (total_sec % 3600) / 60;
    uint32_t s = total_sec % 60;

    if (h > 0)
        swprintf(buf, buflen, L"%02u:%02u:%02u", h, m, s);
    else
        swprintf(buf, buflen, L"%02u:%02u", m, s);
}

static void format_deadline_str(time_t epoch, wchar_t* buf, size_t buflen)
{
    struct tm t;
    localtime_s(&t, &epoch);
    wcsftime(buf, buflen, L"%I:%M %p", &t);
}

static void on_timer_completed_cb(TimerEngine* engine, void* user_data)
{
    (void)engine;
    AppController* app = (AppController*)user_data;
    log_write(LOG_LEVEL_INFO, "Timer completed event triggered!");

    if (app->is_break)
    {
        /* Break finished -> switch to work session */
        app->is_break = false;
        log_write(LOG_LEVEL_INFO, "Break finished. Switching back to work session.");

        if (app->settings.sound_enabled)
        {
            audio_play_completion(app->settings.custom_sound_path);
        }

        if (app->settings.toast_enabled && app->window.hwnd)
        {
            toast_show_notification(app->window.hwnd, L"Timer420 Alert", L"Break finished! Starting work session.");
        }

        uint32_t dur = app->settings.work_seconds;
        timer_engine_set_duration(&app->timer, dur);
        if (app->settings.repeat_enabled)
        {
            timer_engine_start(&app->timer);
            app->deadline_epoch = time(NULL) + dur;
        }
    }
    else
    {
        /* Work session finished -> switch to break session */
        app->is_break = true;
        log_write(LOG_LEVEL_INFO, "Work session completed. Switching to break session.");

        if (app->settings.sound_enabled)
        {
            audio_play_completion(app->settings.custom_sound_path);
        }

        if (app->settings.toast_enabled && app->window.hwnd)
        {
            toast_show_notification(app->window.hwnd, L"Timer420 Alert", L"Work session completed! Starting break.");
        }

        uint32_t breakDur = app->settings.break_seconds;
        if (breakDur == 0) breakDur = 300;
        timer_engine_set_duration(&app->timer, breakDur);
        if (app->settings.repeat_enabled)
        {
            timer_engine_start(&app->timer);
            app->deadline_epoch = time(NULL) + breakDur;
        }
    }

    if (app->window.hwnd) InvalidateRect(app->window.hwnd, NULL, FALSE);
    if (app->controller_win.hwnd && app->controller_win.visible)
    {
        controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
    }
}

static void on_power_event_cb(PowerEvent event, void* user_data)
{
    AppController* app = (AppController*)user_data;
    if (event == POWER_EVENT_RESUME)
    {
        log_write(LOG_LEVEL_INFO, "System resumed from sleep mode. Updating timer engine.");
        timer_engine_update(&app->timer);
        if (app->window.hwnd)
        {
            InvalidateRect(app->window.hwnd, NULL, FALSE);
        }
        if (app->controller_win.hwnd && app->controller_win.visible)
        {
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
        }
    }
}

static COLORREF pick_color_dialog(HWND hwndOwner, COLORREF defaultColor)
{
    CHOOSECOLORW cc = {0};
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwndOwner;
    cc.lpCustColors = g_custColors;
    cc.rgbResult = defaultColor;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColorW(&cc))
    {
        return cc.rgbResult;
    }
    return defaultColor;
}

static LRESULT CALLBACK controller_window_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    AppController* app = app_controller_get_instance();

    switch (msg)
    {
    case WM_ERASEBKGND:
    {
        HDC hdc = (HDC)wParam;
        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, app->controller_win.brush_bg);
        return 1;
    }

    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(226, 232, 240)); /* Light Slate Text */
        SetBkMode(hdc, TRANSPARENT);
        return (INT_PTR)app->controller_win.brush_bg;
    }

    case WM_CTLCOLOREDIT:
    {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(248, 250, 252)); /* Crisp White Text */
        SetBkColor(hdc, RGB(30, 41, 59));      /* Slate 800 background */
        return (INT_PTR)app->controller_win.brush_edit;
    }

    case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG:
    {
        HDC hdc = (HDC)wParam;
        SetTextColor(hdc, RGB(226, 232, 240));
        SetBkColor(hdc, RGB(15, 23, 42));
        return (INT_PTR)app->controller_win.brush_bg;
    }

    case WM_COMMAND:
    {
        WORD code = HIWORD(wParam);
        WORD id   = LOWORD(wParam);

        if (id == ID_CTRL_COMBO_THEMES && code == CBN_SELCHANGE)
        {
            HWND hCombo = (HWND)lParam;
            int sel = (int)SendMessageW(hCombo, CB_GETCURSEL, 0, 0);
            if (sel != CB_ERR)
            {
                app->settings.theme_preset = sel;
                settings_save(&app->settings, NULL);
                if (app->window.hwnd) InvalidateRect(app->window.hwnd, NULL, FALSE);
            }
            return 0;
        }

        /* Filter out edit control change/focus notifications to prevent infinite event loops */
        if (lParam != 0 && (code == EN_CHANGE || code == EN_UPDATE || code == EN_SETFOCUS || code == EN_KILLFOCUS))
        {
            return 0;
        }

        bool handled = true;

        switch (id)
        {
        case ID_CTRL_BTN_STARTPAUSE:
            if (app->timer.state == TIMER_STATE_RUNNING)
                timer_engine_pause(&app->timer);
            else
                timer_engine_start(&app->timer);
            break;

        case ID_CTRL_BTN_STOP:
            timer_engine_stop(&app->timer);
            break;

        case ID_CTRL_BTN_RESET:
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->timer.duration_seconds;
            break;

        case ID_CTRL_BTN_TOGGLE_BREAK:
            app->is_break = !app->is_break;
            {
                uint32_t dur = app->is_break ? app->settings.break_seconds : app->settings.work_seconds;
                if (dur == 0) dur = 300;
                timer_engine_set_duration(&app->timer, dur);
                timer_engine_reset(&app->timer);
                app->deadline_epoch = time(NULL) + dur;
                controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            }
            break;

        case ID_CTRL_BTN_APPLY_TIME: {
            wchar_t hBuf[16] = {0}, mBuf[16] = {0}, sBuf[16] = {0}, rBuf[16] = {0};
            wchar_t bmBuf[16] = {0}, bsBuf[16] = {0};
            GetDlgItemTextW(hwnd, ID_CTRL_EDIT_HOURS, hBuf, 16);
            GetDlgItemTextW(hwnd, ID_CTRL_EDIT_MINS, mBuf, 16);
            GetDlgItemTextW(hwnd, ID_CTRL_EDIT_SECS, sBuf, 16);
            GetDlgItemTextW(hwnd, ID_CTRL_EDIT_BREAK_MINS, bmBuf, 16);
            GetDlgItemTextW(hwnd, ID_CTRL_EDIT_BREAK_SECS, bsBuf, 16);
            GetDlgItemTextW(hwnd, ID_CTRL_EDIT_REPEAT_COUNT, rBuf, 16);
            uint32_t h = (uint32_t)_wtoi(hBuf);
            uint32_t m = (uint32_t)_wtoi(mBuf);
            uint32_t s = (uint32_t)_wtoi(sBuf);
            uint32_t bm = (uint32_t)_wtoi(bmBuf);
            uint32_t bs = (uint32_t)_wtoi(bsBuf);
            uint32_t rep = (uint32_t)_wtoi(rBuf);
            uint32_t totalSec = h * 3600 + m * 60 + s;
            uint32_t breakSec = bm * 60 + bs;
            if (breakSec == 0) breakSec = 300;
            app->settings.break_seconds = breakSec;

            if (totalSec > 0)
            {
                app->settings.work_seconds = totalSec;
                app->settings.repeat_count = rep;
                if (!app->is_break)
                {
                    timer_engine_set_duration(&app->timer, totalSec);
                    timer_engine_reset(&app->timer);
                    app->deadline_epoch = time(NULL) + totalSec;
                }
                settings_save(&app->settings, NULL);
                controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            }
            break;
        }

        case ID_CTRL_BTN_PRESET_15M:
            app->settings.work_seconds = 15 * 60;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_25M:
            app->settings.work_seconds = 25 * 60;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_30M:
            app->settings.work_seconds = 30 * 60;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_45M:
            app->settings.work_seconds = 45 * 60;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_1H:
            app->settings.work_seconds = 1 * 3600;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_2H:
            app->settings.work_seconds = 2 * 3600;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        /* Modern 2026 1-Click Productivity Flow Templates */
        case ID_CTRL_BTN_PRESET_POMODORO:
            app->settings.work_seconds = 25 * 60;
            app->settings.break_seconds = 5 * 60;
            app->is_break = false;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_DEEP:
            app->settings.work_seconds = 50 * 60;
            app->settings.break_seconds = 10 * 60;
            app->is_break = false;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_SPRINT:
            app->settings.work_seconds = 90 * 60;
            app->settings.break_seconds = 15 * 60;
            app->is_break = false;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_BTN_PRESET_FLOW:
            app->settings.work_seconds = 120 * 60;
            app->settings.break_seconds = 20 * 60;
            app->is_break = false;
            timer_engine_set_duration(&app->timer, app->settings.work_seconds);
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->settings.work_seconds;
            settings_save(&app->settings, NULL);
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            break;

        case ID_CTRL_RADIO_TIMER_MODE:
            app->settings.live_clock_mode = false;
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_RADIO_CLOCK_MODE:
            app->settings.live_clock_mode = true;
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_BTN_SYNC_TIME: {
            controller_window_set_sync_status(&app->controller_win, L"Connecting to Internet...");
            TimeSyncResult res = time_sync_fetch_bangladesh_time();
            if (res.success)
            {
                app->settings.clock_time_offset_seconds = res.offset_seconds;
                settings_save(&app->settings, NULL);
            }
            controller_window_set_sync_status(&app->controller_win, res.message);
            break;
        }

        case ID_CTRL_BTN_PICK_TEXT_COLOR:
            app->settings.custom_text_color = pick_color_dialog(hwnd, app->settings.custom_text_color);
            app->settings.theme_preset = 5; /* Custom */
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_BTN_PICK_BG_COLOR:
            app->settings.custom_bg_color = pick_color_dialog(hwnd, app->settings.custom_bg_color);
            app->settings.theme_preset = 5; /* Custom */
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_BTN_PICK_ACCENT_COLOR:
            app->settings.custom_accent_color = pick_color_dialog(hwnd, app->settings.custom_accent_color);
            app->settings.theme_preset = 5; /* Custom */
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_RADIO_MINIMAL:
            app->settings.mode = OVERLAY_MODE_MINIMAL;
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_RADIO_PROGRESS:
            app->settings.mode = OVERLAY_MODE_PROGRESS;
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_RADIO_EDGE:
            app->settings.mode = OVERLAY_MODE_EDGE;
            settings_save(&app->settings, NULL);
            break;

        case ID_CTRL_CHK_OVERLAY_VISIBLE: {
            overlay_window_set_visible(&app->window, !app->window.visible);
            break;
        }

        case ID_CTRL_CHK_ALWAYS_ON_TOP: {
            app->settings.always_on_top = !app->settings.always_on_top;
            if (app->window.hwnd)
            {
                SetWindowPos(app->window.hwnd, app->settings.always_on_top ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            }
            settings_save(&app->settings, NULL);
            break;
        }

        case ID_CTRL_CHK_SOUND_ENABLED: {
            app->settings.sound_enabled = !app->settings.sound_enabled;
            settings_save(&app->settings, NULL);
            break;
        }

        case ID_CTRL_CHK_TOAST_ENABLED: {
            app->settings.toast_enabled = !app->settings.toast_enabled;
            settings_save(&app->settings, NULL);
            break;
        }

        case ID_CTRL_CHK_REPEAT_ENABLED: {
            app->settings.repeat_enabled = !app->settings.repeat_enabled;
            timer_engine_set_repeat(&app->timer, app->settings.repeat_enabled, app->settings.repeat_count);
            settings_save(&app->settings, NULL);
            break;
        }

        case ID_CTRL_BTN_BROWSE_SOUND: {
            OPENFILENAMEW ofn = {0};
            wchar_t szFile[260] = {0};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
            ofn.lpstrFilter = L"Audio Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (GetOpenFileNameW(&ofn))
            {
                wcscpy_s(app->settings.custom_sound_path, 260, szFile);
                SetDlgItemTextW(hwnd, ID_CTRL_EDIT_SOUND_PATH, szFile);
                settings_save(&app->settings, NULL);
            }
            break;
        }

        case ID_CTRL_BTN_TEST_SOUND:
            audio_play_completion(app->settings.custom_sound_path);
            break;

        case ID_CTRL_BTN_CLOSE:
            controller_window_set_visible(&app->controller_win, false);
            break;

        default:
            handled = false;
            break;
        }

        if (handled)
        {
            if (app->window.hwnd) InvalidateRect(app->window.hwnd, NULL, FALSE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
        }
        return 0;
    }

    case WM_CLOSE:
        controller_window_set_visible(&app->controller_win, false);
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

bool app_controller_init(AppController* app, HINSTANCE hInstance)
{
    if (!app || !hInstance) return false;

    logger_init("timer420.log", LOG_LEVEL_INFO);
    log_write(LOG_LEVEL_INFO, "Initializing Timer420 Application Controller Release 2.0...");

    settings_load(&app->settings, NULL);

    Clock clock = clock_create_monotonic();
    timer_engine_init(&app->timer, clock);
    timer_engine_set_duration(&app->timer, app->settings.work_seconds);
    timer_engine_set_repeat(&app->timer, app->settings.repeat_enabled, app->settings.repeat_count);

    app->timer.on_completed = on_timer_completed_cb;
    app->timer.user_data = app;

    if (!overlay_window_create(&app->window, hInstance, app_controller_wndproc))
    {
        log_write(LOG_LEVEL_ERROR, "Failed to create overlay window.");
        return false;
    }

    if (!controller_window_create(&app->controller_win, hInstance, controller_window_wndproc))
    {
        log_write(LOG_LEVEL_ERROR, "Failed to create controller window.");
        return false;
    }

    tray_manager_init(&app->tray, app->window.hwnd);
    win32_hotkeys_register(app->window.hwnd);

    SetTimer(app->window.hwnd, 1, 250, NULL);
    app->deadline_epoch = time(NULL) + app->settings.work_seconds;

    /* Populate duration inputs with initial work_seconds & break_seconds setting */
    controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);

    /* Show Control Center on launch */
    controller_window_set_visible(&app->controller_win, true);
    controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);

    log_write(LOG_LEVEL_INFO, "Timer420 initialized successfully.");
    return true;
}

static void paint_overlay_window(AppController* app, HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBmp = CreateCompatibleBitmap(hdc, w, h);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, memBmp);

    uint32_t remSec = timer_engine_get_remaining_seconds(&app->timer);

    RenderParams params = {0};
    params.hdc = memDC;
    params.client_rect = rc;
    params.remaining_seconds = remSec;
    params.total_seconds = app->timer.duration_seconds;
    params.progress = timer_engine_get_progress(&app->timer);
    params.is_break = app->is_break;
    params.is_running = (app->timer.state == TIMER_STATE_RUNNING);
    params.font_huge = app->window.font_huge;
    params.font_normal = app->window.font_normal;
    params.font_small = app->window.font_small;

    /* 2026 High-Tech Animation Modulation */
    params.pulse_factor = animator_get_pulse(1200);
    params.breathing_factor = animator_get_breathing(3000);
    params.shimmer_phase = animator_get_shimmer_phase(2400);

    /* AI Palette Evaluation */
    if (app->settings.theme_preset == 0)
    {
        /* 🤖 AI Auto Smart Palette */
        params.palette = color_adapter_analyze_desktop(hwnd);
    }
    else
    {
        /* Preset Theme */
        params.palette = color_adapter_get_preset_palette(
            app->settings.theme_preset,
            app->settings.custom_text_color,
            app->settings.custom_bg_color,
            app->settings.custom_accent_color
        );
    }

    if (app->settings.live_clock_mode)
    {
        params.is_live_clock = true;
        time_t bst_epoch = time_sync_get_bst_epoch(app->settings.clock_time_offset_seconds);
        time_sync_format_bst(bst_epoch, params.time_str, 32, params.bst_date_str, 64);
    }
    else
    {
        params.is_live_clock = false;
        format_time_str(remSec, params.time_str, 32);
        format_deadline_str(app->deadline_epoch, params.deadline_str, 32);
    }

    switch (app->settings.mode)
    {
    case OVERLAY_MODE_MINIMAL:  render_minimal(memDC, &params); break;
    case OVERLAY_MODE_EDGE:     render_edge(memDC, &params); break;
    case OVERLAY_MODE_PROGRESS:
    default:                    render_progress(memDC, &params); break;
    }

    BitBlt(hdc, 0, 0, w, h, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBmp);
    DeleteObject(memBmp);
    DeleteDC(memDC);

    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK app_controller_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    AppController* app = app_controller_get_instance();

    switch (msg)
    {
    case WM_TIMER:
        timer_engine_update(&app->timer);
        if (app->timer.state == TIMER_STATE_RUNNING || app->settings.live_clock_mode)
        {
            if (!app->settings.live_clock_mode)
            {
                app->deadline_epoch = time(NULL) + timer_engine_get_remaining_seconds(&app->timer);
            }
            InvalidateRect(hwnd, NULL, FALSE);
        }
        if (app->controller_win.hwnd && app->controller_win.visible)
        {
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
        }
        return 0;

    case WM_PAINT:
        paint_overlay_window(app, hwnd);
        return 0;

    case WM_ERASEBKGND:
        return 1;

    case WM_POWERBROADCAST:
        win32_power_handle_message(wParam, lParam, on_power_event_cb, app);
        return TRUE;

    case WM_HOTKEY:
        switch (wParam)
        {
        case HOTKEY_ID_TOGGLE:
            overlay_window_set_visible(&app->window, !app->window.visible);
            break;
        case HOTKEY_ID_STARTPAUSE:
            if (app->timer.state == TIMER_STATE_RUNNING)
            {
                timer_engine_pause(&app->timer);
            }
            else
            {
                timer_engine_start(&app->timer);
            }
            InvalidateRect(hwnd, NULL, TRUE);
            if (app->controller_win.hwnd && app->controller_win.visible)
            {
                controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            }
            break;
        case HOTKEY_ID_RESET:
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->timer.duration_seconds;
            InvalidateRect(hwnd, NULL, TRUE);
            if (app->controller_win.hwnd && app->controller_win.visible)
            {
                controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            }
            break;
        }
        return 0;

    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP)
        {
            tray_manager_show_menu(&app->tray, hwnd);
        }
        else if (lParam == WM_LBUTTONDBLCLK)
        {
            controller_window_set_visible(&app->controller_win, !app->controller_win.visible);
            if (app->controller_win.visible)
            {
                controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            }
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_TRAY_OPEN_CONTROLLER:
            controller_window_set_visible(&app->controller_win, true);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;

        case ID_TRAY_EXIT:
            DestroyWindow(hwnd);
            break;
        case ID_TRAY_TOGGLE:
            overlay_window_set_visible(&app->window, !app->window.visible);
            break;
        case ID_TRAY_STARTPAUSE:
            if (app->timer.state == TIMER_STATE_RUNNING)
                timer_engine_pause(&app->timer);
            else
                timer_engine_start(&app->timer);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_RESET:
            timer_engine_reset(&app->timer);
            app->deadline_epoch = time(NULL) + app->timer.duration_seconds;
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_SET_2H:
            app->is_break = false;
            timer_engine_set_duration(&app->timer, 2 * 3600);
            app->settings.work_seconds = 2 * 3600;
            settings_save(&app->settings, NULL);
            app->deadline_epoch = time(NULL) + app->timer.duration_seconds;
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_SET_1H:
            app->is_break = false;
            timer_engine_set_duration(&app->timer, 1 * 3600);
            app->settings.work_seconds = 1 * 3600;
            settings_save(&app->settings, NULL);
            app->deadline_epoch = time(NULL) + app->timer.duration_seconds;
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_SET_30M:
            app->is_break = false;
            timer_engine_set_duration(&app->timer, 30 * 60);
            app->settings.work_seconds = 30 * 60;
            settings_save(&app->settings, NULL);
            app->deadline_epoch = time(NULL) + app->timer.duration_seconds;
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_SET_15M:
            app->is_break = false;
            timer_engine_set_duration(&app->timer, 15 * 60);
            app->settings.work_seconds = 15 * 60;
            settings_save(&app->settings, NULL);
            app->deadline_epoch = time(NULL) + app->timer.duration_seconds;
            controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_MODE_MINIMAL:
            app->settings.mode = OVERLAY_MODE_MINIMAL;
            settings_save(&app->settings, NULL);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_MODE_PROGRESS:
            app->settings.mode = OVERLAY_MODE_PROGRESS;
            settings_save(&app->settings, NULL);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_MODE_EDGE:
            app->settings.mode = OVERLAY_MODE_EDGE;
            settings_save(&app->settings, NULL);
            InvalidateRect(hwnd, NULL, TRUE);
            controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            break;
        case ID_TRAY_TOGGLE_BREAK:
            app->is_break = !app->is_break;
            {
                uint32_t dur = app->is_break ? app->settings.break_seconds : app->settings.work_seconds;
                if (dur == 0) dur = 300;
                timer_engine_set_duration(&app->timer, dur);
                timer_engine_reset(&app->timer);
                app->deadline_epoch = time(NULL) + dur;
                controller_window_set_duration_inputs(&app->controller_win, app->settings.work_seconds, app->settings.break_seconds);
                InvalidateRect(hwnd, NULL, TRUE);
                controller_window_update_ui(&app->controller_win, &app->timer, &app->settings, app->is_break);
            }
            break;
        case ID_TRAY_TEST_SOUND:
            audio_play_completion(app->settings.custom_sound_path);
            break;
        }
        return 0;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        app_controller_shutdown(app);
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void app_controller_shutdown(AppController* app)
{
    if (!app) return;

    log_write(LOG_LEVEL_INFO, "Shutting down Timer420 Application Controller...");
    win32_hotkeys_unregister(app->window.hwnd);
    tray_manager_cleanup(&app->tray);
    overlay_window_destroy_fonts(&app->window);
    controller_window_destroy(&app->controller_win);
    settings_save(&app->settings, NULL);
    logger_close();
}

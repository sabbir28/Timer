#include "ui/windows/controller_window.h"
#include "services/logging/logger.h"
#include <commctrl.h>
#include <commdlg.h>
#include <stdio.h>
#include <wchar.h>

#define CONTROLLER_CLASS_NAME L"Timer420ControllerWindowClass"

static HWND g_hTimeDisplay = NULL;
static HWND g_hStatusDisplay = NULL;
static HWND g_hProgressBar = NULL;

static HWND g_hEditHours = NULL;
static HWND g_hSpinHours = NULL;
static HWND g_hEditMins = NULL;
static HWND g_hSpinMins = NULL;
static HWND g_hEditSecs = NULL;
static HWND g_hSpinSecs = NULL;

static HWND g_hRadioMinimal = NULL;
static HWND g_hRadioProgress = NULL;
static HWND g_hRadioEdge = NULL;
static HWND g_hChkOverlayVis = NULL;
static HWND g_hChkAlwaysOnTop = NULL;

static HWND g_hRadioTimerMode = NULL;
static HWND g_hRadioClockMode = NULL;
static HWND g_hBtnSyncTime = NULL;
static HWND g_hStaticSyncStatus = NULL;

static HWND g_hComboThemes = NULL;
static HWND g_hBtnPickText = NULL;
static HWND g_hBtnPickBg = NULL;
static HWND g_hBtnPickAccent = NULL;

static HWND g_hChkSound = NULL;
static HWND g_hEditSoundPath = NULL;
static HWND g_hChkToast = NULL;
static HWND g_hChkRepeat = NULL;
static HWND g_hEditRepeatCount = NULL;

static bool s_is_updating_ui = false;

static void create_controls(HWND hwnd, ControllerWindow* win)
{
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

    /* UI Font setup */
    win->font_title = CreateFontW(
        -20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI"
    );

    win->font_time = CreateFontW(
        -32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI"
    );

    win->font_ui = CreateFontW(
        -14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI"
    );

    /* 1. Timer Status Group */
    HWND hGrp1 = CreateWindowExW(0, L"BUTTON", L" Timer Status & Progress ",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        15, 10, 475, 105, hwnd, NULL, hInst, NULL);
    SendMessageW(hGrp1, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hTimeDisplay = CreateWindowExW(0, L"STATIC", L"00:00:00",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        30, 32, 220, 42, hwnd, NULL, hInst, NULL);
    SendMessageW(g_hTimeDisplay, WM_SETFONT, (WPARAM)win->font_time, TRUE);

    g_hStatusDisplay = CreateWindowExW(0, L"STATIC", L"STOPPED",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        260, 40, 210, 24, hwnd, NULL, hInst, NULL);
    SendMessageW(g_hStatusDisplay, WM_SETFONT, (WPARAM)win->font_title, TRUE);

    g_hProgressBar = CreateWindowExW(0, PROGRESS_CLASSW, NULL,
        WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
        30, 80, 440, 22, hwnd, NULL, hInst, NULL);
    SendMessageW(g_hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 100));

    /* Action Buttons */
    HWND hBtnStart = CreateWindowExW(0, L"BUTTON", L"▶ Start / ⏸ Pause",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        15, 125, 150, 34, hwnd, (HMENU)ID_CTRL_BTN_STARTPAUSE, hInst, NULL);
    SendMessageW(hBtnStart, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    HWND hBtnStop = CreateWindowExW(0, L"BUTTON", L"⏹ Stop",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        175, 125, 150, 34, hwnd, (HMENU)ID_CTRL_BTN_STOP, hInst, NULL);
    SendMessageW(hBtnStop, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    HWND hBtnReset = CreateWindowExW(0, L"BUTTON", L"↺ Reset",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        335, 125, 155, 34, hwnd, (HMENU)ID_CTRL_BTN_RESET, hInst, NULL);
    SendMessageW(hBtnReset, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* 2. Custom Duration Group */
    HWND hGrp2 = CreateWindowExW(0, L"BUTTON", L" Custom Timer Duration ",
        WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
        15, 170, 475, 85, hwnd, NULL, hInst, NULL);
    SendMessageW(hGrp2, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* Hours */
    HWND hLblH = CreateWindowExW(0, L"STATIC", L"Hours:", WS_CHILD | WS_VISIBLE, 30, 205, 45, 20, hwnd, NULL, hInst, NULL);
    SendMessageW(hLblH, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hEditHours = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT, 80, 202, 45, 24, hwnd, (HMENU)ID_CTRL_EDIT_HOURS, hInst, NULL);
    SendMessageW(g_hEditHours, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hSpinHours = CreateWindowExW(0, UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS, 0, 0, 0, 0, hwnd, (HMENU)ID_CTRL_SPIN_HOURS, hInst, NULL);
    SendMessageW(g_hSpinHours, UDM_SETBUDDY, (WPARAM)g_hEditHours, 0);
    SendMessageW(g_hSpinHours, UDM_SETRANGE32, 0, 99);

    /* Mins */
    HWND hLblM = CreateWindowExW(0, L"STATIC", L"Mins:", WS_CHILD | WS_VISIBLE, 145, 205, 40, 20, hwnd, NULL, hInst, NULL);
    SendMessageW(hLblM, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hEditMins = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"25", WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT, 190, 202, 45, 24, hwnd, (HMENU)ID_CTRL_EDIT_MINS, hInst, NULL);
    SendMessageW(g_hEditMins, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hSpinMins = CreateWindowExW(0, UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS, 0, 0, 0, 0, hwnd, (HMENU)ID_CTRL_SPIN_MINS, hInst, NULL);
    SendMessageW(g_hSpinMins, UDM_SETBUDDY, (WPARAM)g_hEditMins, 0);
    SendMessageW(g_hSpinMins, UDM_SETRANGE32, 0, 59);

    /* Secs */
    HWND hLblS = CreateWindowExW(0, L"STATIC", L"Secs:", WS_CHILD | WS_VISIBLE, 255, 205, 40, 20, hwnd, NULL, hInst, NULL);
    SendMessageW(hLblS, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hEditSecs = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT, 300, 202, 45, 24, hwnd, (HMENU)ID_CTRL_EDIT_SECS, hInst, NULL);
    SendMessageW(g_hEditSecs, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hSpinSecs = CreateWindowExW(0, UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS, 0, 0, 0, 0, hwnd, (HMENU)ID_CTRL_SPIN_SECS, hInst, NULL);
    SendMessageW(g_hSpinSecs, UDM_SETBUDDY, (WPARAM)g_hEditSecs, 0);
    SendMessageW(g_hSpinSecs, UDM_SETRANGE32, 0, 59);

    /* Apply button */
    HWND hBtnApply = CreateWindowExW(0, L"BUTTON", L"Set Time", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 370, 200, 105, 28, hwnd, (HMENU)ID_CTRL_BTN_APPLY_TIME, hInst, NULL);
    SendMessageW(hBtnApply, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* 3. Quick Time Presets Group */
    HWND hGrp3 = CreateWindowExW(0, L"BUTTON", L" Quick Time Presets ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 15, 265, 475, 80, hwnd, NULL, hInst, NULL);
    SendMessageW(hGrp3, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    HWND hP15 = CreateWindowExW(0, L"BUTTON", L"15 Mins", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 295, 70, 32, hwnd, (HMENU)ID_CTRL_BTN_PRESET_15M, hInst, NULL);
    SendMessageW(hP15, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    HWND hP25 = CreateWindowExW(0, L"BUTTON", L"25 Mins (Pomodoro)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 108, 295, 135, 32, hwnd, (HMENU)ID_CTRL_BTN_PRESET_25M, hInst, NULL);
    SendMessageW(hP25, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    HWND hP30 = CreateWindowExW(0, L"BUTTON", L"30 Mins", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 295, 70, 32, hwnd, (HMENU)ID_CTRL_BTN_PRESET_30M, hInst, NULL);
    SendMessageW(hP30, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    HWND hP45 = CreateWindowExW(0, L"BUTTON", L"45 Mins", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 328, 295, 70, 32, hwnd, (HMENU)ID_CTRL_BTN_PRESET_45M, hInst, NULL);
    SendMessageW(hP45, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    HWND hP1H = CreateWindowExW(0, L"BUTTON", L"1 Hour", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 406, 295, 70, 32, hwnd, (HMENU)ID_CTRL_BTN_PRESET_1H, hInst, NULL);
    SendMessageW(hP1H, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* 4. Bangladesh Live Clock & Internet Sync Group */
    HWND hGrpBD = CreateWindowExW(0, L"BUTTON", L" 🇧🇩 Bangladesh Live Clock & Internet Sync ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 15, 355, 475, 95, hwnd, NULL, hInst, NULL);
    SendMessageW(hGrpBD, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hRadioTimerMode = CreateWindowExW(0, L"BUTTON", L"Countdown Timer Mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 30, 382, 170, 22, hwnd, (HMENU)ID_CTRL_RADIO_TIMER_MODE, hInst, NULL);
    SendMessageW(g_hRadioTimerMode, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hRadioClockMode = CreateWindowExW(0, L"BUTTON", L"Bangladesh Live Clock (BST UTC+6)", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 210, 382, 260, 22, hwnd, (HMENU)ID_CTRL_RADIO_CLOCK_MODE, hInst, NULL);
    SendMessageW(g_hRadioClockMode, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hBtnSyncTime = CreateWindowExW(0, L"BUTTON", L"🌐 Sync Time (Asia/Dhaka)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 413, 200, 26, hwnd, (HMENU)ID_CTRL_BTN_SYNC_TIME, hInst, NULL);
    SendMessageW(g_hBtnSyncTime, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hStaticSyncStatus = CreateWindowExW(0, L"STATIC", L"Ready to sync with internet", WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, 240, 418, 235, 20, hwnd, (HMENU)ID_CTRL_STATIC_SYNC_STATUS, hInst, NULL);
    SendMessageW(g_hStaticSyncStatus, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* 5. AI Color Themes & Palette Picker Group */
    HWND hGrpAI = CreateWindowExW(0, L"BUTTON", L" 🎨 AI Color Themes & Custom RGB Picker ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 15, 460, 475, 95, hwnd, NULL, hInst, NULL);
    SendMessageW(hGrpAI, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    HWND hLblTheme = CreateWindowExW(0, L"STATIC", L"Theme Preset:", WS_CHILD | WS_VISIBLE, 30, 488, 95, 20, hwnd, NULL, hInst, NULL);
    SendMessageW(hLblTheme, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hComboThemes = CreateWindowExW(0, L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL, 130, 485, 210, 150, hwnd, (HMENU)ID_CTRL_COMBO_THEMES, hInst, NULL);
    SendMessageW(g_hComboThemes, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    SendMessageW(g_hComboThemes, CB_ADDSTRING, 0, (LPARAM)L"🤖 AI Auto-Palette (Smart Contrast)");
    SendMessageW(g_hComboThemes, CB_ADDSTRING, 0, (LPARAM)L"🌆 Cyberpunk Neon");
    SendMessageW(g_hComboThemes, CB_ADDSTRING, 0, (LPARAM)L"🇧🇩 Emerald Barishal");
    SendMessageW(g_hComboThemes, CB_ADDSTRING, 0, (LPARAM)L"🕶 Dark Obsidian");
    SendMessageW(g_hComboThemes, CB_ADDSTRING, 0, (LPARAM)L"☀️ Solar Gold");
    SendMessageW(g_hComboThemes, CB_ADDSTRING, 0, (LPARAM)L"🎨 Custom 24-Bit RGB");
    SendMessageW(g_hComboThemes, CB_SETCURSEL, 0, 0);

    g_hBtnPickText = CreateWindowExW(0, L"BUTTON", L"Text Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 30, 520, 100, 26, hwnd, (HMENU)ID_CTRL_BTN_PICK_TEXT_COLOR, hInst, NULL);
    SendMessageW(g_hBtnPickText, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hBtnPickBg = CreateWindowExW(0, L"BUTTON", L"BG Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 140, 520, 100, 26, hwnd, (HMENU)ID_CTRL_BTN_PICK_BG_COLOR, hInst, NULL);
    SendMessageW(g_hBtnPickBg, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hBtnPickAccent = CreateWindowExW(0, L"BUTTON", L"Accent Color", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 520, 100, 26, hwnd, (HMENU)ID_CTRL_BTN_PICK_ACCENT_COLOR, hInst, NULL);
    SendMessageW(g_hBtnPickAccent, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* 6. Display & Overlay Options Group */
    HWND hGrp4 = CreateWindowExW(0, L"BUTTON", L" Display & Overlay Options ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 15, 565, 475, 95, hwnd, NULL, hInst, NULL);
    SendMessageW(hGrp4, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hRadioMinimal = CreateWindowExW(0, L"BUTTON", L"Minimal Overlay", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 30, 593, 130, 22, hwnd, (HMENU)ID_CTRL_RADIO_MINIMAL, hInst, NULL);
    SendMessageW(g_hRadioMinimal, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hRadioProgress = CreateWindowExW(0, L"BUTTON", L"Progress Bar", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 170, 593, 120, 22, hwnd, (HMENU)ID_CTRL_RADIO_PROGRESS, hInst, NULL);
    SendMessageW(g_hRadioProgress, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hRadioEdge = CreateWindowExW(0, L"BUTTON", L"Screen Edge Border", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 300, 593, 170, 22, hwnd, (HMENU)ID_CTRL_RADIO_EDGE, hInst, NULL);
    SendMessageW(g_hRadioEdge, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hChkOverlayVis = CreateWindowExW(0, L"BUTTON", L"Show Floating Overlay Window", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 30, 623, 220, 22, hwnd, (HMENU)ID_CTRL_CHK_OVERLAY_VISIBLE, hInst, NULL);
    SendMessageW(g_hChkOverlayVis, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    g_hChkAlwaysOnTop = CreateWindowExW(0, L"BUTTON", L"Keep Overlay Always On Top", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 260, 623, 210, 22, hwnd, (HMENU)ID_CTRL_CHK_ALWAYS_ON_TOP, hInst, NULL);
    SendMessageW(g_hChkAlwaysOnTop, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* 7. Audio & Notifications Group */
    HWND hGrp5 = CreateWindowExW(0, L"BUTTON", L" Audio & Notifications ", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 15, 670, 475, 110, hwnd, NULL, hInst, NULL);
    SendMessageW(hGrp5, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hChkSound = CreateWindowExW(0, L"BUTTON", L"Play Alarm Sound on Completion", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 30, 695, 230, 22, hwnd, (HMENU)ID_CTRL_CHK_SOUND_ENABLED, hInst, NULL);
    SendMessageW(g_hChkSound, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hEditSoundPath = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 260, 695, 140, 24, hwnd, (HMENU)ID_CTRL_EDIT_SOUND_PATH, hInst, NULL);
    SendMessageW(g_hEditSoundPath, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
    HWND hBtnBrowseSound = CreateWindowExW(0, L"BUTTON", L"Browse...", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 405, 694, 70, 26, hwnd, (HMENU)ID_CTRL_BTN_BROWSE_SOUND, hInst, NULL);
    SendMessageW(hBtnBrowseSound, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hChkToast = CreateWindowExW(0, L"BUTTON", L"Show Windows Notification Toast", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 30, 725, 240, 22, hwnd, (HMENU)ID_CTRL_CHK_TOAST_ENABLED, hInst, NULL);
    SendMessageW(g_hChkToast, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hChkRepeat = CreateWindowExW(0, L"BUTTON", L"Auto Repeat", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 280, 725, 100, 22, hwnd, (HMENU)ID_CTRL_CHK_REPEAT_ENABLED, hInst, NULL);
    SendMessageW(g_hChkRepeat, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    g_hEditRepeatCount = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"0", WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT, 390, 725, 45, 24, hwnd, (HMENU)ID_CTRL_EDIT_REPEAT_COUNT, hInst, NULL);
    SendMessageW(g_hEditRepeatCount, WM_SETFONT, (WPARAM)win->font_ui, TRUE);

    /* Close Button */
    HWND hBtnClose = CreateWindowExW(0, L"BUTTON", L"Close Control Center", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 175, 790, 160, 32, hwnd, (HMENU)ID_CTRL_BTN_CLOSE, hInst, NULL);
    SendMessageW(hBtnClose, WM_SETFONT, (WPARAM)win->font_ui, TRUE);
}

bool controller_window_create(ControllerWindow* win, HINSTANCE hInstance, WNDPROC wndProc)
{
    if (!win || !hInstance || !wndProc) return false;

    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = wndProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = CONTROLLER_CLASS_NAME;

    RegisterClassExW(&wc);

    int width = 520;
    int height = 875;
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenW - width) / 2;
    int y = (screenH - height) / 2;

    win->hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_DLGMODALFRAME,
        CONTROLLER_CLASS_NAME,
        L"Timer420 – Control Center",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        x, y, width, height,
        NULL, NULL, hInstance, NULL
    );

    if (!win->hwnd) return false;

    create_controls(win->hwnd, win);
    win->visible = false;
    return true;
}

void controller_window_set_visible(ControllerWindow* win, bool visible)
{
    if (!win || !win->hwnd || !IsWindow(win->hwnd)) return;
    win->visible = visible;
    ShowWindow(win->hwnd, visible ? SW_SHOW : SW_HIDE);
    if (visible)
    {
        SetForegroundWindow(win->hwnd);
    }
}

void controller_window_set_duration_inputs(ControllerWindow* win, uint32_t total_seconds)
{
    if (!win || !win->hwnd || !IsWindow(win->hwnd)) return;

    uint32_t h = total_seconds / 3600;
    uint32_t m = (total_seconds % 3600) / 60;
    uint32_t s = total_seconds % 60;

    wchar_t buf[16];
    if (g_hEditHours && IsWindow(g_hEditHours)) { swprintf(buf, 16, L"%u", h); SetWindowTextW(g_hEditHours, buf); }
    if (g_hEditMins && IsWindow(g_hEditMins))   { swprintf(buf, 16, L"%u", m); SetWindowTextW(g_hEditMins, buf); }
    if (g_hEditSecs && IsWindow(g_hEditSecs))   { swprintf(buf, 16, L"%u", s); SetWindowTextW(g_hEditSecs, buf); }
}

void controller_window_set_sync_status(ControllerWindow* win, const wchar_t* status_msg)
{
    (void)win;
    if (g_hStaticSyncStatus && IsWindow(g_hStaticSyncStatus) && status_msg)
    {
        SetWindowTextW(g_hStaticSyncStatus, status_msg);
    }
}

void controller_window_update_ui(ControllerWindow* win, const TimerEngine* timer, const AppSettings* settings)
{
    if (!win || !win->hwnd || !IsWindow(win->hwnd) || !timer || !settings) return;
    if (s_is_updating_ui) return;
    s_is_updating_ui = true;

    /* Remaining Time */
    if (g_hTimeDisplay && IsWindow(g_hTimeDisplay))
    {
        uint32_t remSec = timer_engine_get_remaining_seconds(timer);
        uint32_t h = remSec / 3600;
        uint32_t m = (remSec % 3600) / 60;
        uint32_t s = remSec % 60;

        wchar_t timeBuf[32];
        swprintf(timeBuf, 32, L"%02u:%02u:%02u", h, m, s);
        SetWindowTextW(g_hTimeDisplay, timeBuf);
    }

    /* Timer State */
    if (g_hStatusDisplay && IsWindow(g_hStatusDisplay))
    {
        const wchar_t* stateStr = L"STOPPED";
        if (settings->live_clock_mode)
        {
            stateStr = L"🇧🇩 LIVE BST";
        }
        else
        {
            switch (timer->state)
            {
            case TIMER_STATE_RUNNING:   stateStr = L"▶ RUNNING"; break;
            case TIMER_STATE_PAUSED:    stateStr = L"⏸ PAUSED"; break;
            case TIMER_STATE_COMPLETED: stateStr = L"✔ FINISHED"; break;
            case TIMER_STATE_STOPPED:
            default:                    stateStr = L"⏹ STOPPED"; break;
            }
        }
        SetWindowTextW(g_hStatusDisplay, stateStr);
    }

    /* Progress bar */
    if (g_hProgressBar && IsWindow(g_hProgressBar))
    {
        float progressPct = timer_engine_get_progress(timer) * 100.0f;
        if (progressPct > 100.0f) progressPct = 100.0f;
        if (progressPct < 0.0f) progressPct = 0.0f;
        SendMessageW(g_hProgressBar, PBM_SETPOS, (WPARAM)(int)progressPct, 0);
    }

    /* Bangladesh Mode Radios */
    if (g_hRadioTimerMode && IsWindow(g_hRadioTimerMode))
        SendMessageW(g_hRadioTimerMode, BM_SETCHECK, (!settings->live_clock_mode) ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_hRadioClockMode && IsWindow(g_hRadioClockMode))
        SendMessageW(g_hRadioClockMode, BM_SETCHECK, (settings->live_clock_mode) ? BST_CHECKED : BST_UNCHECKED, 0);

    /* Theme combobox */
    if (g_hComboThemes && IsWindow(g_hComboThemes))
        SendMessageW(g_hComboThemes, CB_SETCURSEL, settings->theme_preset, 0);

    /* Radio buttons */
    if (g_hRadioMinimal && IsWindow(g_hRadioMinimal))
        SendMessageW(g_hRadioMinimal, BM_SETCHECK, (settings->mode == OVERLAY_MODE_MINIMAL) ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_hRadioProgress && IsWindow(g_hRadioProgress))
        SendMessageW(g_hRadioProgress, BM_SETCHECK, (settings->mode == OVERLAY_MODE_PROGRESS) ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_hRadioEdge && IsWindow(g_hRadioEdge))
        SendMessageW(g_hRadioEdge, BM_SETCHECK, (settings->mode == OVERLAY_MODE_EDGE) ? BST_CHECKED : BST_UNCHECKED, 0);

    /* Checkboxes */
    if (g_hChkOverlayVis && IsWindow(g_hChkOverlayVis))
        SendMessageW(g_hChkOverlayVis, BM_SETCHECK, win->visible ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_hChkSound && IsWindow(g_hChkSound))
        SendMessageW(g_hChkSound, BM_SETCHECK, settings->sound_enabled ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_hChkToast && IsWindow(g_hChkToast))
        SendMessageW(g_hChkToast, BM_SETCHECK, settings->toast_enabled ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_hChkAlwaysOnTop && IsWindow(g_hChkAlwaysOnTop))
        SendMessageW(g_hChkAlwaysOnTop, BM_SETCHECK, settings->always_on_top ? BST_CHECKED : BST_UNCHECKED, 0);
    if (g_hChkRepeat && IsWindow(g_hChkRepeat))
        SendMessageW(g_hChkRepeat, BM_SETCHECK, settings->repeat_enabled ? BST_CHECKED : BST_UNCHECKED, 0);

    HWND focusWnd = GetFocus();
    if (g_hEditRepeatCount && IsWindow(g_hEditRepeatCount) && focusWnd != g_hEditRepeatCount)
    {
        wchar_t rbuf[16];
        swprintf(rbuf, 16, L"%u", settings->repeat_count);
        SetWindowTextW(g_hEditRepeatCount, rbuf);
    }

    if (g_hEditSoundPath && IsWindow(g_hEditSoundPath) && focusWnd != g_hEditSoundPath)
    {
        SetWindowTextW(g_hEditSoundPath, settings->custom_sound_path);
    }

    s_is_updating_ui = false;
}

void controller_window_destroy(ControllerWindow* win)
{
    if (!win) return;
    if (win->font_title) DeleteObject(win->font_title);
    if (win->font_time)  DeleteObject(win->font_time);
    if (win->font_ui)    DeleteObject(win->font_ui);
    if (win->hwnd && IsWindow(win->hwnd)) DestroyWindow(win->hwnd);

    win->hwnd = NULL;
    win->visible = false;

    g_hTimeDisplay = NULL;
    g_hStatusDisplay = NULL;
    g_hProgressBar = NULL;
    g_hEditHours = NULL;
    g_hSpinHours = NULL;
    g_hEditMins = NULL;
    g_hSpinMins = NULL;
    g_hEditSecs = NULL;
    g_hSpinSecs = NULL;
    g_hRadioTimerMode = NULL;
    g_hRadioClockMode = NULL;
    g_hBtnSyncTime = NULL;
    g_hStaticSyncStatus = NULL;
    g_hComboThemes = NULL;
    g_hBtnPickText = NULL;
    g_hBtnPickBg = NULL;
    g_hBtnPickAccent = NULL;
    g_hRadioMinimal = NULL;
    g_hRadioProgress = NULL;
    g_hRadioEdge = NULL;
    g_hChkOverlayVis = NULL;
    g_hChkAlwaysOnTop = NULL;
    g_hChkSound = NULL;
    g_hEditSoundPath = NULL;
    g_hChkToast = NULL;
    g_hChkRepeat = NULL;
    g_hEditRepeatCount = NULL;
}

#ifndef TIMER420_CONTROLLER_WINDOW_H
#define TIMER420_CONTROLLER_WINDOW_H

#include "core/timer/timer_engine.h"
#include "core/config/settings.h"
#include <windows.h>
#include <stdbool.h>

/* Window Control IDs */
#define ID_CTRL_BTN_STARTPAUSE      2001
#define ID_CTRL_BTN_STOP            2002
#define ID_CTRL_BTN_RESET           2003
#define ID_CTRL_BTN_TOGGLE_BREAK    2004

#define ID_CTRL_EDIT_HOURS          2010
#define ID_CTRL_SPIN_HOURS         2011
#define ID_CTRL_EDIT_MINS           2012
#define ID_CTRL_SPIN_MINS          2013
#define ID_CTRL_EDIT_SECS           2014
#define ID_CTRL_SPIN_SECS          2015
#define ID_CTRL_EDIT_BREAK_MINS     2017
#define ID_CTRL_SPIN_BREAK_MINS     2018
#define ID_CTRL_EDIT_BREAK_SECS     2019
#define ID_CTRL_SPIN_BREAK_SECS     2026
#define ID_CTRL_BTN_APPLY_TIME      2016

#define ID_CTRL_BTN_PRESET_15M      2020
#define ID_CTRL_BTN_PRESET_25M      2021
#define ID_CTRL_BTN_PRESET_30M      2022
#define ID_CTRL_BTN_PRESET_45M      2023
#define ID_CTRL_BTN_PRESET_1H       2024
#define ID_CTRL_BTN_PRESET_2H       2025

#define ID_CTRL_BTN_PRESET_POMODORO 2080
#define ID_CTRL_BTN_PRESET_DEEP     2081
#define ID_CTRL_BTN_PRESET_SPRINT   2082
#define ID_CTRL_BTN_PRESET_FLOW     2083

#define ID_CTRL_RADIO_MINIMAL       2030
#define ID_CTRL_RADIO_PROGRESS      2031
#define ID_CTRL_RADIO_EDGE          2032
#define ID_CTRL_CHK_OVERLAY_VISIBLE 2033
#define ID_CTRL_CHK_ALWAYS_ON_TOP   2034

#define ID_CTRL_CHK_SOUND_ENABLED   2040
#define ID_CTRL_EDIT_SOUND_PATH     2041
#define ID_CTRL_BTN_BROWSE_SOUND    2042
#define ID_CTRL_BTN_TEST_SOUND      2046
#define ID_CTRL_CHK_TOAST_ENABLED   2043
#define ID_CTRL_CHK_REPEAT_ENABLED  2044
#define ID_CTRL_EDIT_REPEAT_COUNT   2045

#define ID_CTRL_RADIO_TIMER_MODE    2060
#define ID_CTRL_RADIO_CLOCK_MODE    2061
#define ID_CTRL_BTN_SYNC_TIME       2062
#define ID_CTRL_STATIC_SYNC_STATUS  2063

#define ID_CTRL_COMBO_THEMES        2070
#define ID_CTRL_BTN_PICK_TEXT_COLOR 2071
#define ID_CTRL_BTN_PICK_BG_COLOR   2072
#define ID_CTRL_BTN_PICK_ACCENT_COLOR 2073

#define ID_CTRL_BTN_CLOSE           2050

typedef struct ControllerWindow {
    HWND hwnd;
    HFONT font_title;
    HFONT font_time;
    HFONT font_ui;
    HBRUSH brush_bg;
    HBRUSH brush_card;
    HBRUSH brush_edit;
    bool visible;
} ControllerWindow;

/**
 * Creates the Controller Center GUI window.
 */
bool controller_window_create(ControllerWindow* win, HINSTANCE hInstance, WNDPROC wndProc);

/**
 * Shows or hides the Controller Center GUI window.
 */
void controller_window_set_visible(ControllerWindow* win, bool visible);

/**
 * Explicitly populates the Work and Break duration inputs.
 */
void controller_window_set_duration_inputs(ControllerWindow* win, uint32_t work_seconds, uint32_t break_seconds);

/**
 * Updates the sync status text label in the GUI.
 */
void controller_window_set_sync_status(ControllerWindow* win, const wchar_t* status_msg);

/**
 * Synchronizes controls with current timer state, settings, and session mode.
 */
void controller_window_update_ui(ControllerWindow* win, const TimerEngine* timer, const AppSettings* settings, bool is_break);

/**
 * Destroys window resources and fonts.
 */
void controller_window_destroy(ControllerWindow* win);

#endif /* TIMER420_CONTROLLER_WINDOW_H */

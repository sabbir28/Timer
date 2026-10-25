#ifndef TIMER420_APP_CONTROLLER_H
#define TIMER420_APP_CONTROLLER_H

#include "core/timer/timer_engine.h"
#include "core/config/settings.h"
#include "ui/windows/overlay_window.h"
#include "ui/windows/controller_window.h"
#include "ui/tray/tray_manager.h"
#include <windows.h>

typedef struct AppController {
    TimerEngine timer;
    AppSettings settings;
    OverlayWindow window;
    ControllerWindow controller_win;
    TrayManager tray;
    time_t deadline_epoch;
    bool is_break;
} AppController;

/**
 * Global application controller instance accessor.
 */
AppController* app_controller_get_instance(void);

/**
 * Initializes the application controller, settings, clock, and UI components.
 */
bool app_controller_init(AppController* app, HINSTANCE hInstance);

/**
 * Windows Window Procedure handling window messages for Timer420.
 */
LRESULT CALLBACK app_controller_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**
 * Cleans up application resources upon shutdown.
 */
void app_controller_shutdown(AppController* app);

#endif /* TIMER420_APP_CONTROLLER_H */

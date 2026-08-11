#ifndef TIMER420_TRAY_MANAGER_H
#define TIMER420_TRAY_MANAGER_H

#include <windows.h>
#include <shellapi.h>
#include <stdbool.h>

#define WM_TRAYICON (WM_USER + 1)

#define ID_TRAY_OPEN_CONTROLLER  1000
#define ID_TRAY_EXIT             1001
#define ID_TRAY_TOGGLE           1002
#define ID_TRAY_STARTPAUSE       1003
#define ID_TRAY_RESET            1004
#define ID_TRAY_SET_2H           1005
#define ID_TRAY_SET_1H           1006
#define ID_TRAY_SET_30M          1007
#define ID_TRAY_SET_15M          1008
#define ID_TRAY_MODE_MINIMAL     1010
#define ID_TRAY_MODE_PROGRESS    1011
#define ID_TRAY_MODE_EDGE        1012

typedef struct TrayManager {
    NOTIFYICONDATAW nid;
    HMENU hTrayMenu;
} TrayManager;

/**
 * Initializes and registers system tray icon and populates context menu.
 */
bool tray_manager_init(TrayManager* tray, HWND hwnd);

/**
 * Displays system tray popup menu at current mouse position.
 */
void tray_manager_show_menu(TrayManager* tray, HWND hwnd);

/**
 * Removes system tray icon and destroys menu resources.
 */
void tray_manager_cleanup(TrayManager* tray);

#endif /* TIMER420_TRAY_MANAGER_H */

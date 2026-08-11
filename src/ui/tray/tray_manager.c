#include "ui/tray/tray_manager.h"
#include <shellapi.h>

bool tray_manager_init(TrayManager* tray, HWND hwnd)
{
    if (!tray || !hwnd) return false;

    ZeroMemory(&tray->nid, sizeof(tray->nid));
    tray->nid.cbSize           = sizeof(NOTIFYICONDATAW);
    tray->nid.hWnd             = hwnd;
    tray->nid.uID              = 1;
    tray->nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    tray->nid.uCallbackMessage = WM_TRAYICON;
    tray->nid.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wcscpy_s(tray->nid.szTip, 128, L"Timer420 – Work Timer");

    if (!Shell_NotifyIconW(NIM_ADD, &tray->nid))
    {
        return false;
    }

    tray->hTrayMenu = CreatePopupMenu();
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_OPEN_CONTROLLER, L"Open Control Center...\tCtrl+Alt+C");
    AppendMenuW(tray->hTrayMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_TOGGLE,     L"Show / Hide Overlay\tCtrl+Shift+Space");
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_STARTPAUSE, L"Start / Pause\tCtrl+Shift+S");
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_RESET,      L"Reset\tCtrl+Shift+R");
    AppendMenuW(tray->hTrayMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_SET_2H,     L"Set 2 hours");
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_SET_1H,     L"Set 1 hour");
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_SET_30M,    L"Set 30 minutes");
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_SET_15M,    L"Set 15 minutes");
    AppendMenuW(tray->hTrayMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_MODE_MINIMAL,  L"Mode: Minimal");
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_MODE_PROGRESS, L"Mode: Progress");
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_MODE_EDGE,     L"Mode: Edge Border");
    AppendMenuW(tray->hTrayMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(tray->hTrayMenu, MF_STRING, ID_TRAY_EXIT, L"Exit");

    return true;
}

void tray_manager_show_menu(TrayManager* tray, HWND hwnd)
{
    if (!tray || !tray->hTrayMenu || !hwnd) return;

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hwnd);
    TrackPopupMenu(tray->hTrayMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    PostMessage(hwnd, WM_NULL, 0, 0);
}

void tray_manager_cleanup(TrayManager* tray)
{
    if (!tray) return;

    Shell_NotifyIconW(NIM_DELETE, &tray->nid);
    if (tray->hTrayMenu)
    {
        DestroyMenu(tray->hTrayMenu);
        tray->hTrayMenu = NULL;
    }
}

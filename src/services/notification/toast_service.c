#include "services/notification/toast_service.h"
#include <shellapi.h>

bool toast_show_notification(HWND hwnd, const wchar_t* title, const wchar_t* message)
{
    if (!hwnd || !title || !message) return false;

    NOTIFYICONDATAW nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATAW);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_INFO;
    nid.dwInfoFlags = NIIF_INFO;

    wcscpy_s(nid.szInfoTitle, 64, title);
    wcscpy_s(nid.szInfo, 256, message);

    return Shell_NotifyIconW(NIM_MODIFY, &nid) != FALSE;
}

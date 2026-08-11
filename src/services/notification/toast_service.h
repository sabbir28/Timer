#ifndef TIMER420_TOAST_SERVICE_H
#define TIMER420_TOAST_SERVICE_H

#include <windows.h>
#include <stdbool.h>

/**
 * Displays a Windows Tray Toast notification.
 */
bool toast_show_notification(HWND hwnd, const wchar_t* title, const wchar_t* message);

#endif /* TIMER420_TOAST_SERVICE_H */

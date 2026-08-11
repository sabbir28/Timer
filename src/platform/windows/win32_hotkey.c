#include "platform/windows/win32_hotkey.h"

bool win32_hotkeys_register(HWND hwnd)
{
    if (!hwnd) return false;

    bool ok = true;
    ok &= RegisterHotKey(hwnd, HOTKEY_ID_TOGGLE,     MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, VK_SPACE) != FALSE;
    ok &= RegisterHotKey(hwnd, HOTKEY_ID_STARTPAUSE, MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, 'S') != FALSE;
    ok &= RegisterHotKey(hwnd, HOTKEY_ID_RESET,      MOD_CONTROL | MOD_SHIFT | MOD_NOREPEAT, 'R') != FALSE;

    return ok;
}

void win32_hotkeys_unregister(HWND hwnd)
{
    if (!hwnd) return;
    UnregisterHotKey(hwnd, HOTKEY_ID_TOGGLE);
    UnregisterHotKey(hwnd, HOTKEY_ID_STARTPAUSE);
    UnregisterHotKey(hwnd, HOTKEY_ID_RESET);
}

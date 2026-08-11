#ifndef TIMER420_WIN32_HOTKEY_H
#define TIMER420_WIN32_HOTKEY_H

#include <windows.h>
#include <stdbool.h>

#define HOTKEY_ID_TOGGLE     1
#define HOTKEY_ID_STARTPAUSE 2
#define HOTKEY_ID_RESET      3

/**
 * Registers application global hotkeys (Ctrl+Shift+Space, Ctrl+Shift+S, Ctrl+Shift+R).
 * Fixes global ESC key trap by excluding VK_ESCAPE from global OS hotkey registration.
 */
bool win32_hotkeys_register(HWND hwnd);

/**
 * Unregisters all global hotkeys registered to the specified window.
 */
void win32_hotkeys_unregister(HWND hwnd);

#endif /* TIMER420_WIN32_HOTKEY_H */

#ifndef TIMER420_WIN32_POWER_H
#define TIMER420_WIN32_POWER_H

#include <windows.h>
#include <stdbool.h>

typedef enum PowerEvent {
    POWER_EVENT_SUSPEND = 0,
    POWER_EVENT_RESUME
} PowerEvent;

typedef void (*PowerEventCallback)(PowerEvent event, void* user_data);

/**
 * Processes WM_POWERBROADCAST message and invokes callback on sleep or resume.
 */
bool win32_power_handle_message(WPARAM wParam, LPARAM lParam, PowerEventCallback callback, void* user_data);

#endif /* TIMER420_WIN32_POWER_H */

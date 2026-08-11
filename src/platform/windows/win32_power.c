#include "platform/windows/win32_power.h"

bool win32_power_handle_message(WPARAM wParam, LPARAM lParam, PowerEventCallback callback, void* user_data)
{
    (void)lParam;
    if (!callback) return false;

    switch (wParam)
    {
    case PBT_APMSUSPEND:
        callback(POWER_EVENT_SUSPEND, user_data);
        return true;

    case PBT_APMRESUMESUSPEND:
    case PBT_APMRESUMEAUTOMATIC:
        callback(POWER_EVENT_RESUME, user_data);
        return true;

    default:
        return false;
    }
}

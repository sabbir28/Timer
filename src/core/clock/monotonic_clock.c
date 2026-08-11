#include "core/clock/clock.h"
#include <windows.h>

static TimestampMs monotonic_win32_get_ms(void* user_data)
{
    (void)user_data;
    LARGE_INTEGER freq;
    LARGE_INTEGER counter;

    if (!QueryPerformanceFrequency(&freq) || freq.QuadPart == 0)
    {
        return (TimestampMs)GetTickCount64();
    }

    QueryPerformanceCounter(&counter);
    return (TimestampMs)((counter.QuadPart * 1000ULL) / freq.QuadPart);
}

TimestampMs clock_get_monotonic_ms(const Clock* clock)
{
    if (!clock || !clock->get_monotonic_ms)
    {
        return 0;
    }
    return clock->get_monotonic_ms(clock->user_data);
}

Clock clock_create_monotonic(void)
{
    Clock c;
    c.get_monotonic_ms = monotonic_win32_get_ms;
    c.user_data = NULL;
    return c;
}

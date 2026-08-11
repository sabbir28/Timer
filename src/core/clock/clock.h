#ifndef TIMER420_CLOCK_H
#define TIMER420_CLOCK_H

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t TimestampMs;

typedef struct Clock {
    TimestampMs (*get_monotonic_ms)(void* user_data);
    void* user_data;
} Clock;

/**
 * Gets the current monotonic timestamp in milliseconds from the specified clock.
 * Returns 0 if clock or function pointer is NULL.
 */
TimestampMs clock_get_monotonic_ms(const Clock* clock);

/**
 * Default Win32 QueryPerformanceCounter monotonic clock factory.
 */
Clock clock_create_monotonic(void);

#endif /* TIMER420_CLOCK_H */

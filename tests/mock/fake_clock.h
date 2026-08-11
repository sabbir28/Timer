#ifndef TIMER420_FAKE_CLOCK_H
#define TIMER420_FAKE_CLOCK_H

#include "core/clock/clock.h"

typedef struct FakeClockData {
    TimestampMs current_time_ms;
} FakeClockData;

/**
 * Creates a controllable FakeClock initialized to initial_time_ms.
 */
Clock fake_clock_create(TimestampMs initial_time_ms);

/**
 * Advances the FakeClock by advance_ms milliseconds instantly.
 */
void fake_clock_advance_ms(Clock* clock, uint64_t advance_ms);

/**
 * Advances the FakeClock by seconds.
 */
void fake_clock_advance_seconds(Clock* clock, uint32_t seconds);

/**
 * Advances the FakeClock by hours.
 */
void fake_clock_advance_hours(Clock* clock, uint32_t hours);

#endif /* TIMER420_FAKE_CLOCK_H */

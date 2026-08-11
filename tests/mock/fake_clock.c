#include "tests/mock/fake_clock.h"
#include <stdlib.h>

static TimestampMs fake_clock_get_ms(void* user_data)
{
    FakeClockData* data = (FakeClockData*)user_data;
    if (!data) return 0;
    return data->current_time_ms;
}

Clock fake_clock_create(TimestampMs initial_time_ms)
{
    FakeClockData* data = (FakeClockData*)malloc(sizeof(FakeClockData));
    data->current_time_ms = initial_time_ms;

    Clock c;
    c.get_monotonic_ms = fake_clock_get_ms;
    c.user_data = data;
    return c;
}

void fake_clock_advance_ms(Clock* clock, uint64_t advance_ms)
{
    if (!clock || !clock->user_data) return;
    FakeClockData* data = (FakeClockData*)clock->user_data;
    data->current_time_ms += advance_ms;
}

void fake_clock_advance_seconds(Clock* clock, uint32_t seconds)
{
    fake_clock_advance_ms(clock, (uint64_t)seconds * 1000ULL);
}

void fake_clock_advance_hours(Clock* clock, uint32_t hours)
{
    fake_clock_advance_ms(clock, (uint64_t)hours * 3600ULL * 1000ULL);
}

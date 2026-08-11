#include "core/clock/clock.h"
#include "tests/mock/fake_clock.h"
#include <stdio.h>
#include <assert.h>

void test_clock(void)
{
    printf("Running test_clock...\n");

    Clock fake = fake_clock_create(1000);
    assert(clock_get_monotonic_ms(&fake) == 1000);

    fake_clock_advance_ms(&fake, 500);
    assert(clock_get_monotonic_ms(&fake) == 1500);

    fake_clock_advance_seconds(&fake, 10);
    assert(clock_get_monotonic_ms(&fake) == 11500);

    fake_clock_advance_hours(&fake, 2);
    assert(clock_get_monotonic_ms(&fake) == (11500 + 2 * 3600 * 1000));

    printf("PASSED: test_clock\n");
}

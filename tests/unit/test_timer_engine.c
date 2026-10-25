#include "core/timer/timer_engine.h"
#include "tests/mock/fake_clock.h"
#include <stdio.h>
#include <assert.h>

static bool g_completed_fired = false;

static void on_test_completed(TimerEngine* engine, void* user_data)
{
    (void)engine;
    (void)user_data;
    g_completed_fired = true;
}

void test_timer_engine(void)
{
    printf("Running test_timer_engine...\n");

    Clock fake = fake_clock_create(1000);
    TimerEngine engine;
    timer_engine_init(&engine, fake);

    /* 2 hours = 7200 seconds */
    bool ok = timer_engine_set_duration(&engine, 7200);
    assert(ok);
    (void)ok;
    assert(engine.duration_seconds == 7200);
    assert(engine.remaining_ms == 7200000ULL);

    engine.on_completed = on_test_completed;

    /* Start timer */
    assert(timer_engine_start(&engine));
    assert(engine.state == TIMER_STATE_RUNNING);

    /* Advance clock by 1 hour (3600 seconds) in milliseconds instantly */
    fake_clock_advance_hours(&engine.clock, 1);
    timer_engine_update(&engine);

    assert(engine.state == TIMER_STATE_RUNNING);
    assert(timer_engine_get_remaining_seconds(&engine) == 3600);
    assert(!g_completed_fired);

    /* Pause timer */
    assert(timer_engine_pause(&engine));
    assert(engine.state == TIMER_STATE_PAUSED);

    /* Advance clock while paused -> remaining time must stay frozen */
    fake_clock_advance_hours(&engine.clock, 1);
    timer_engine_update(&engine);
    assert(timer_engine_get_remaining_seconds(&engine) == 3600);

    /* Resume timer */
    assert(timer_engine_resume(&engine));
    assert(engine.state == TIMER_STATE_RUNNING);

    /* Advance remaining 1 hour */
    fake_clock_advance_hours(&engine.clock, 1);
    timer_engine_update(&engine);

    /* Should reach completion instantly */
    assert(engine.state == TIMER_STATE_COMPLETED);
    assert(timer_engine_get_remaining_seconds(&engine) == 0);
    assert(g_completed_fired);

    printf("PASSED: test_timer_engine (2-hour instant advancement verified)\n");
}

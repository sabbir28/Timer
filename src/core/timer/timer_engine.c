#include "core/timer/timer_engine.h"
#include <string.h>

void timer_engine_init(TimerEngine* engine, Clock clock)
{
    if (!engine) return;
    memset(engine, 0, sizeof(TimerEngine));
    engine->clock = clock;
    engine->state = TIMER_STATE_IDLE;
    engine->current_cycle = 1;
}

bool timer_engine_set_duration(TimerEngine* engine, uint32_t seconds)
{
    if (!engine || seconds == 0) return false;

    engine->duration_seconds = seconds;
    engine->duration_ms = (uint64_t)seconds * 1000ULL;
    engine->remaining_ms = engine->duration_ms;

    if (engine->state == TIMER_STATE_IDLE || engine->state == TIMER_STATE_STOPPED)
    {
        engine->remaining_ms = engine->duration_ms;
    }
    return true;
}

void timer_engine_set_repeat(TimerEngine* engine, bool enabled, uint32_t max_cycles)
{
    if (!engine) return;
    engine->repeat_enabled = enabled;
    engine->max_cycles = max_cycles;
}

bool timer_engine_start(TimerEngine* engine)
{
    if (!engine || engine->duration_ms == 0) return false;

    if (!timer_state_can_transition(engine->state, TIMER_STATE_RUNNING))
    {
        return false;
    }

    TimestampMs now = clock_get_monotonic_ms(&engine->clock);
    engine->last_update_ms = now;

    if (engine->state == TIMER_STATE_PAUSED)
    {
        engine->target_timestamp_ms = now + engine->paused_remaining_ms;
    }
    else
    {
        engine->target_timestamp_ms = now + engine->duration_ms;
        engine->remaining_ms = engine->duration_ms;
    }

    engine->state = TIMER_STATE_RUNNING;
    return true;
}

bool timer_engine_pause(TimerEngine* engine)
{
    if (!engine || engine->state != TIMER_STATE_RUNNING) return false;

    TimestampMs now = clock_get_monotonic_ms(&engine->clock);
    if (now >= engine->target_timestamp_ms)
    {
        engine->paused_remaining_ms = 0;
        engine->remaining_ms = 0;
    }
    else
    {
        engine->paused_remaining_ms = engine->target_timestamp_ms - now;
        engine->remaining_ms = engine->paused_remaining_ms;
    }

    engine->state = TIMER_STATE_PAUSED;
    return true;
}

bool timer_engine_resume(TimerEngine* engine)
{
    if (!engine || engine->state != TIMER_STATE_PAUSED) return false;
    return timer_engine_start(engine);
}

bool timer_engine_stop(TimerEngine* engine)
{
    if (!engine) return false;
    engine->state = TIMER_STATE_STOPPED;
    engine->remaining_ms = engine->duration_ms;
    engine->paused_remaining_ms = 0;
    engine->completed_cycles = 0;
    engine->current_cycle = 1;
    return true;
}

bool timer_engine_reset(TimerEngine* engine)
{
    if (!engine) return false;
    engine->state = TIMER_STATE_IDLE;
    engine->remaining_ms = engine->duration_ms;
    engine->paused_remaining_ms = 0;
    return true;
}

void timer_engine_update(TimerEngine* engine)
{
    if (!engine || engine->state != TIMER_STATE_RUNNING) return;

    TimestampMs now = clock_get_monotonic_ms(&engine->clock);

    if (now >= engine->target_timestamp_ms)
    {
        engine->remaining_ms = 0;
        engine->completed_cycles++;
        engine->state = TIMER_STATE_COMPLETED;

        if (engine->on_completed)
        {
            engine->on_completed(engine, engine->user_data);
        }

        if (engine->repeat_enabled && (engine->max_cycles == 0 || engine->current_cycle < engine->max_cycles))
        {
            engine->current_cycle++;
            timer_engine_start(engine);
        }
    }
    else
    {
        engine->remaining_ms = engine->target_timestamp_ms - now;
        if (engine->on_tick)
        {
            engine->on_tick(engine, engine->user_data);
        }
    }
}

uint32_t timer_engine_get_remaining_seconds(const TimerEngine* engine)
{
    if (!engine) return 0;
    return (uint32_t)((engine->remaining_ms + 999ULL) / 1000ULL);
}

float timer_engine_get_progress(const TimerEngine* engine)
{
    if (!engine || engine->duration_ms == 0) return 0.0f;
    float pct = (float)engine->remaining_ms / (float)engine->duration_ms;
    if (pct < 0.0f) pct = 0.0f;
    if (pct > 1.0f) pct = 1.0f;
    return pct;
}

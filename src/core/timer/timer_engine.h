#ifndef TIMER420_TIMER_ENGINE_H
#define TIMER420_TIMER_ENGINE_H

#include "core/clock/clock.h"
#include "core/state/timer_state.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct TimerEngine TimerEngine;

typedef void (*TimerCompletedCallback)(TimerEngine* engine, void* user_data);
typedef void (*TimerTickCallback)(TimerEngine* engine, void* user_data);

struct TimerEngine {
    Clock clock;
    TimerState state;

    uint32_t duration_seconds;
    uint64_t duration_ms;
    uint64_t remaining_ms;

    TimestampMs last_update_ms;
    TimestampMs target_timestamp_ms;
    uint64_t paused_remaining_ms;

    bool repeat_enabled;
    uint32_t max_cycles;        /* 0 = infinite */
    uint32_t current_cycle;
    uint32_t completed_cycles;

    TimerCompletedCallback on_completed;
    TimerTickCallback on_tick;
    void* user_data;
};

/**
 * Initializes the timer engine with a specified clock provider.
 */
void timer_engine_init(TimerEngine* engine, Clock clock);

/**
 * Sets the target timer duration in seconds.
 */
bool timer_engine_set_duration(TimerEngine* engine, uint32_t seconds);

/**
 * Enables or disables automatic repetition and configures max cycles.
 */
void timer_engine_set_repeat(TimerEngine* engine, bool enabled, uint32_t max_cycles);

/**
 * Starts or resumes the countdown timer.
 */
bool timer_engine_start(TimerEngine* engine);

/**
 * Pauses an active timer.
 */
bool timer_engine_pause(TimerEngine* engine);

/**
 * Resumes a paused timer.
 */
bool timer_engine_resume(TimerEngine* engine);

/**
 * Stops the timer and resets cycle counters.
 */
bool timer_engine_stop(TimerEngine* engine);

/**
 * Resets the remaining time to full duration.
 */
bool timer_engine_reset(TimerEngine* engine);

/**
 * Ticks the timer engine using current monotonic time. Emits callbacks as needed.
 */
void timer_engine_update(TimerEngine* engine);

/**
 * Gets remaining time formatted in seconds (rounded up).
 */
uint32_t timer_engine_get_remaining_seconds(const TimerEngine* engine);

/**
 * Calculates remaining progress ratio from 0.0 (empty) to 1.0 (full).
 */
float timer_engine_get_progress(const TimerEngine* engine);

#endif /* TIMER420_TIMER_ENGINE_H */

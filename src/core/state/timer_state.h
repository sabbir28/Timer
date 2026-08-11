#ifndef TIMER420_TIMER_STATE_H
#define TIMER420_TIMER_STATE_H

#include <stdbool.h>

typedef enum TimerState {
    TIMER_STATE_IDLE = 0,
    TIMER_STATE_RUNNING,
    TIMER_STATE_PAUSED,
    TIMER_STATE_COMPLETED,
    TIMER_STATE_STOPPED
} TimerState;

/**
 * Returns a human-readable string representation of the TimerState.
 */
const char* timer_state_to_string(TimerState state);

/**
 * Validates whether a state transition from `current` to `target` is valid.
 */
bool timer_state_can_transition(TimerState current, TimerState target);

#endif /* TIMER420_TIMER_STATE_H */

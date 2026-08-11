#include "core/state/timer_state.h"

const char* timer_state_to_string(TimerState state)
{
    switch (state)
    {
    case TIMER_STATE_IDLE:      return "IDLE";
    case TIMER_STATE_RUNNING:   return "RUNNING";
    case TIMER_STATE_PAUSED:    return "PAUSED";
    case TIMER_STATE_COMPLETED: return "COMPLETED";
    case TIMER_STATE_STOPPED:   return "STOPPED";
    default:                    return "UNKNOWN";
    }
}

bool timer_state_can_transition(TimerState current, TimerState target)
{
    if (current == target)
    {
        return true;
    }

    switch (current)
    {
    case TIMER_STATE_IDLE:
        return (target == TIMER_STATE_RUNNING || target == TIMER_STATE_STOPPED);

    case TIMER_STATE_RUNNING:
        return (target == TIMER_STATE_PAUSED ||
                target == TIMER_STATE_COMPLETED ||
                target == TIMER_STATE_STOPPED ||
                target == TIMER_STATE_IDLE);

    case TIMER_STATE_PAUSED:
        return (target == TIMER_STATE_RUNNING ||
                target == TIMER_STATE_STOPPED ||
                target == TIMER_STATE_IDLE);

    case TIMER_STATE_COMPLETED:
        return (target == TIMER_STATE_RUNNING ||  /* repeat cycle */
                target == TIMER_STATE_IDLE ||
                target == TIMER_STATE_STOPPED);

    case TIMER_STATE_STOPPED:
        return (target == TIMER_STATE_IDLE || target == TIMER_STATE_RUNNING);

    default:
        return false;
    }
}

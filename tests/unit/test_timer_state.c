#include "core/state/timer_state.h"
#include <stdio.h>
#include <assert.h>

void test_timer_state(void)
{
    printf("Running test_timer_state...\n");

    assert(timer_state_can_transition(TIMER_STATE_IDLE, TIMER_STATE_RUNNING));
    assert(timer_state_can_transition(TIMER_STATE_RUNNING, TIMER_STATE_PAUSED));
    assert(timer_state_can_transition(TIMER_STATE_PAUSED, TIMER_STATE_RUNNING));
    assert(timer_state_can_transition(TIMER_STATE_RUNNING, TIMER_STATE_COMPLETED));

    /* Valid state string checks */
    assert(timer_state_to_string(TIMER_STATE_RUNNING) != NULL);

    printf("PASSED: test_timer_state\n");
}

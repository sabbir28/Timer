#include "core/config/settings.h"
#include <stdio.h>
#include <assert.h>

void test_settings(void)
{
    printf("Running test_settings...\n");

    AppSettings s;
    settings_set_defaults(&s);

    assert(s.work_seconds == 7200);
    assert(s.break_seconds == 600);
    assert(s.mode == OVERLAY_MODE_PROGRESS);
    assert(s.repeat_enabled == true);

    s.work_seconds = 2; /* Invalid low duration */
    settings_validate(&s);
    assert(s.work_seconds == 5);

    printf("PASSED: test_settings\n");
}

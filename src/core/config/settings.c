#include "core/config/settings.h"
#include <string.h>

void settings_set_defaults(AppSettings* settings)
{
    if (!settings) return;
    memset(settings, 0, sizeof(AppSettings));

    settings->work_seconds = 2 * 60 * 60;   /* 2 hours */
    settings->break_seconds = 10 * 60;      /* 10 minutes */
    settings->mode = OVERLAY_MODE_PROGRESS;
    settings->repeat_enabled = true;
    settings->repeat_count = 0;             /* Infinite */

    settings->sound_enabled = true;
    settings->toast_enabled = true;
    settings->custom_sound_path[0] = L'\0';

    settings->always_on_top = true;
    settings->start_with_windows = false;
    settings->auto_start = false;

    settings->live_clock_mode = false;
    settings->clock_time_offset_seconds = 0;

    settings->theme_preset = 0;             /* 0 = AI Auto Smart Palette */
    settings->custom_text_color = RGB(255, 255, 255);
    settings->custom_bg_color = RGB(15, 23, 42);
    settings->custom_accent_color = RGB(0, 240, 255);
}

void settings_validate(AppSettings* settings)
{
    if (!settings) return;

    if (settings->work_seconds < 5) settings->work_seconds = 5;
    if (settings->work_seconds > 86400) settings->work_seconds = 86400; /* 24 hours max */

    if (settings->break_seconds < 5) settings->break_seconds = 5;
    if (settings->break_seconds > 14400) settings->break_seconds = 14400;

    if (settings->mode > OVERLAY_MODE_EDGE)
    {
        settings->mode = OVERLAY_MODE_PROGRESS;
    }

    if (settings->theme_preset < 0 || settings->theme_preset > 5)
    {
        settings->theme_preset = 0;
    }
}

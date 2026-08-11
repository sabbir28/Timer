#ifndef TIMER420_SETTINGS_H
#define TIMER420_SETTINGS_H

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>

typedef enum OverlayMode {
    OVERLAY_MODE_MINIMAL = 0,
    OVERLAY_MODE_PROGRESS,
    OVERLAY_MODE_EDGE
} OverlayMode;

typedef struct AppSettings {
    uint32_t work_seconds;
    uint32_t break_seconds;
    OverlayMode mode;
    bool repeat_enabled;
    uint32_t repeat_count;     /* 0 = infinite */

    bool sound_enabled;
    bool toast_enabled;
    wchar_t custom_sound_path[260];

    bool always_on_top;
    bool start_with_windows;
    bool auto_start;

    /* Bangladesh Live Clock & Time Sync */
    bool live_clock_mode;              /* true = display Bangladesh Live Clock instead of timer */
    int64_t clock_time_offset_seconds; /* offset from system clock to synced UTC */

    /* Release 2.0 AI Color & Theme System */
    int theme_preset;                  /* 0 = AI Auto, 1 = Cyberpunk, 2 = Emerald, 3 = Obsidian, 4 = Solar, 5 = Custom */
    COLORREF custom_text_color;
    COLORREF custom_bg_color;
    COLORREF custom_accent_color;
} AppSettings;

/**
 * Initializes AppSettings with default values.
 */
void settings_set_defaults(AppSettings* settings);

/**
 * Validates and sanitizes settings to ensure reasonable values.
 */
void settings_validate(AppSettings* settings);

#endif /* TIMER420_SETTINGS_H */

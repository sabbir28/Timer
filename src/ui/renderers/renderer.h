#ifndef TIMER420_RENDERER_H
#define TIMER420_RENDERER_H

#include <windows.h>
#include "core/config/settings.h"
#include "services/color/color_adapter.h"
#include <stdint.h>

typedef struct RenderParams {
    HDC hdc;
    RECT client_rect;
    uint32_t remaining_seconds;
    uint32_t total_seconds;
    float progress;
    bool is_break;
    bool is_running;
    bool is_live_clock;
    float pulse_factor; /* 0.0 to 1.0 for animations */
    ColorPalette palette;
    wchar_t time_str[32];
    wchar_t deadline_str[32];
    wchar_t bst_date_str[64];
    HFONT font_huge;
    HFONT font_normal;
    HFONT font_small;
} RenderParams;

void render_minimal(HDC hdc, const RenderParams* params);
void render_progress(HDC hdc, const RenderParams* params);
void render_edge(HDC hdc, const RenderParams* params);

#endif /* TIMER420_RENDERER_H */

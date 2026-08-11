#ifndef TIMER420_COLOR_ADAPTER_H
#define TIMER420_COLOR_ADAPTER_H

#include <windows.h>
#include <stdbool.h>

typedef struct ColorPalette {
    COLORREF background_color;
    COLORREF text_color;
    COLORREF accent_color;
    COLORREF border_color;
    bool is_dark_bg;
} ColorPalette;

/**
 * Analyzes the desktop wallpaper directly under the specified window
 * and synthesizes an optimal high-contrast AI color palette.
 */
ColorPalette color_adapter_analyze_desktop(HWND hwnd);

/**
 * Generates preset color palettes for predefined themes.
 */
ColorPalette color_adapter_get_preset_palette(int preset_id, COLORREF custom_text, COLORREF custom_bg, COLORREF custom_accent);

#endif /* TIMER420_COLOR_ADAPTER_H */

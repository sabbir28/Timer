#ifndef TIMER420_OVERLAY_WINDOW_H
#define TIMER420_OVERLAY_WINDOW_H

#include <windows.h>
#include <stdbool.h>
#include "core/config/settings.h"

typedef struct OverlayWindow {
    HWND hwnd;
    HINSTANCE hInst;
    BOOL visible;
    HFONT font_huge;
    HFONT font_normal;
    HFONT font_small;
} OverlayWindow;

/**
 * Initializes fonts for overlay window text rendering.
 */
void overlay_window_create_fonts(OverlayWindow* window);

/**
 * Destroys overlay window fonts.
 */
void overlay_window_destroy_fonts(OverlayWindow* window);

/**
 * Registers window class and creates layered topmost overlay window.
 */
bool overlay_window_create(OverlayWindow* window, HINSTANCE hInst, WNDPROC wndProc);

/**
 * Toggles or sets overlay window visibility.
 */
void overlay_window_set_visible(OverlayWindow* window, bool visible);

#endif /* TIMER420_OVERLAY_WINDOW_H */

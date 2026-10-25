#ifndef TIMER420_ANIMATOR_H
#define TIMER420_ANIMATOR_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Returns a normalized sine-wave pulse value [0.0, 1.0] for animation cycles.
 */
float animator_get_pulse(uint32_t speed_ms);

/**
 * Smoothly interpolates between two COLORREF colors based on factor t [0.0, 1.0].
 */
COLORREF animator_interpolate_color(COLORREF color_a, COLORREF color_b, float factor);

#endif /* TIMER420_ANIMATOR_H */

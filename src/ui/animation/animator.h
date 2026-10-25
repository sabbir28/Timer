#ifndef TIMER420_ANIMATOR_H
#define TIMER420_ANIMATOR_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * Normalized sine-wave pulse [0.0, 1.0] for animation cycles.
 */
float animator_get_pulse(uint32_t speed_ms);

/**
 * 2026 Organic Biological Breathing Curve [0.0, 1.0]
 * Uses an asymmetric respiratory curve (gentle ease-in inhale, smooth lingering exhale).
 */
float animator_get_breathing(uint32_t period_ms);

/**
 * 2026 Travelling Shimmer / Neon Wave Phase [0.0, 1.0]
 * Produces a continuous scanning beam across progress boundaries and borders.
 */
float animator_get_shimmer_phase(uint32_t cycle_ms);

/**
 * Modern fluid easing curves for 2026 transitions.
 */
float animator_ease_out_cubic(float t);
float animator_ease_in_out_sine(float t);
float animator_ease_out_quad(float t);

/**
 * Smooth exponential dampening towards target across delta time.
 */
float animator_smooth_damp(float current, float target, float smooth_time, float dt);

/**
 * Smoothly interpolates between two COLORREF colors based on factor t [0.0, 1.0].
 */
COLORREF animator_interpolate_color(COLORREF color_a, COLORREF color_b, float factor);

/**
 * Enhances a base color with a high-luminescence neon glow core.
 */
COLORREF animator_glow_color(COLORREF base_color, float intensity);

#endif /* TIMER420_ANIMATOR_H */

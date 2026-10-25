#include "ui/animation/animator.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float animator_get_pulse(uint32_t speed_ms)
{
    if (speed_ms == 0) speed_ms = 1000;
    DWORD now = GetTickCount();
    double cycle = (double)(now % speed_ms) / (double)speed_ms;
    return (float)((sin(cycle * 2.0 * M_PI) + 1.0) * 0.5);
}

float animator_get_breathing(uint32_t period_ms)
{
    if (period_ms == 0) period_ms = 3000;
    DWORD now = GetTickCount();
    double t = (double)(now % period_ms) / (double)period_ms;
    /* 2026 organic respiratory curve: asymmetric inhale (45%) and exhale (55%) */
    double val;
    if (t < 0.45)
    {
        double sub = t / 0.45;
        val = 0.5 * (1.0 - cos(sub * M_PI));
    }
    else
    {
        double sub = (t - 0.45) / 0.55;
        val = 0.5 * (1.0 + cos(sub * M_PI));
    }
    return (float)val;
}

float animator_get_shimmer_phase(uint32_t cycle_ms)
{
    if (cycle_ms == 0) cycle_ms = 2500;
    DWORD now = GetTickCount();
    return (float)((double)(now % cycle_ms) / (double)cycle_ms);
}

float animator_ease_out_cubic(float t)
{
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    float p = 1.0f - t;
    return 1.0f - p * p * p;
}

float animator_ease_in_out_sine(float t)
{
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return (float)(-(cos(M_PI * t) - 1.0) * 0.5);
}

float animator_ease_out_quad(float t)
{
    if (t <= 0.0f) return 0.0f;
    if (t >= 1.0f) return 1.0f;
    return t * (2.0f - t);
}

float animator_smooth_damp(float current, float target, float smooth_time, float dt)
{
    if (smooth_time <= 0.0f) return target;
    float omega = 2.0f / smooth_time;
    float x = omega * dt;
    float exp_val = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    float change = current - target;
    return target + change * exp_val;
}

COLORREF animator_interpolate_color(COLORREF color_a, COLORREF color_b, float factor)
{
    if (factor <= 0.0f) return color_a;
    if (factor >= 1.0f) return color_b;

    int rA = GetRValue(color_a);
    int gA = GetGValue(color_a);
    int bA = GetBValue(color_a);

    int rB = GetRValue(color_b);
    int gB = GetGValue(color_b);
    int bB = GetBValue(color_b);

    int r = rA + (int)((rB - rA) * factor);
    int g = gA + (int)((gB - gA) * factor);
    int b = bA + (int)((bB - bA) * factor);

    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;

    return RGB(r, g, b);
}

COLORREF animator_glow_color(COLORREF base_color, float intensity)
{
    if (intensity <= 0.0f) return base_color;
    int r = GetRValue(base_color);
    int g = GetGValue(base_color);
    int b = GetBValue(base_color);

    r = (int)(r + (255 - r) * (intensity * 0.65f));
    g = (int)(g + (255 - g) * (intensity * 0.65f));
    b = (int)(b + (255 - b) * (intensity * 0.65f));

    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;

    return RGB(r, g, b);
}

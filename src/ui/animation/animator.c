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

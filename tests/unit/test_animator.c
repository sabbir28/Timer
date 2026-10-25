#include "ui/animation/animator.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>

void test_animator(void)
{
    printf("[TEST] Running Animator 2026 Engine Unit Tests...\n");

    /* Test breathing calculation */
    float b1 = animator_get_breathing(3000);
    assert(b1 >= 0.0f && b1 <= 1.0f);
    (void)b1;

    /* Test shimmer phase calculation */
    float s1 = animator_get_shimmer_phase(2500);
    assert(s1 >= 0.0f && s1 <= 1.0f);
    (void)s1;

    /* Test easing functions */
    float e1 = animator_ease_out_cubic(0.0f);
    float e2 = animator_ease_out_cubic(1.0f);
    assert(fabsf(e1 - 0.0f) < 0.001f);
    assert(fabsf(e2 - 1.0f) < 0.001f);
    (void)e1; (void)e2;

    float e3 = animator_ease_in_out_sine(0.0f);
    float e4 = animator_ease_in_out_sine(1.0f);
    assert(fabsf(e3 - 0.0f) < 0.001f);
    assert(fabsf(e4 - 1.0f) < 0.001f);
    (void)e3; (void)e4;

    /* Test color interpolation */
    COLORREF c1 = RGB(0, 0, 0);
    COLORREF c2 = RGB(100, 200, 50);
    COLORREF mid = animator_interpolate_color(c1, c2, 0.5f);
    assert(GetRValue(mid) == 50);
    assert(GetGValue(mid) == 100);
    assert(GetBValue(mid) == 25);
    (void)mid;

    /* Test glow color computation */
    COLORREF glow = animator_glow_color(RGB(100, 100, 100), 1.5f);
    assert(GetRValue(glow) == 150);
    assert(GetGValue(glow) == 150);
    assert(GetBValue(glow) == 150);
    (void)glow;

    printf("[PASS] Animator 2026 Engine Unit Tests passed!\n");
}

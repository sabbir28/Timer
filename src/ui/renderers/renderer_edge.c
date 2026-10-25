#include "ui/renderers/renderer.h"
#include "ui/animation/animator.h"

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

void render_edge(HDC hdc, const RenderParams* params)
{
    int w = params->client_rect.right - params->client_rect.left;
    int h = params->client_rect.bottom - params->client_rect.top;

    /* Background Fill */
    HBRUSH bgBrush = CreateSolidBrush(params->palette.background_color);
    FillRect(hdc, &params->client_rect, bgBrush);
    DeleteObject(bgBrush);

    int borderThickness = 6;
    float elapsedProgress = 1.0f - params->progress;
    if (elapsedProgress < 0.0f) elapsedProgress = 0.0f;
    if (elapsedProgress > 1.0f) elapsedProgress = 1.0f;

    /* Dynamic 2026 Color Palette */
    COLORREF mainColor = params->palette.accent_color;
    if (params->is_break)
    {
        mainColor = animator_interpolate_color(RGB(16, 185, 129), RGB(52, 211, 153), params->breathing_factor);
    }
    else if (params->remaining_seconds <= 60)
    {
        mainColor = animator_interpolate_color(RGB(239, 68, 68), RGB(255, 120, 120), params->pulse_factor);
    }
    else
    {
        mainColor = animator_interpolate_color(mainColor, RGB(56, 189, 248), params->breathing_factor * 0.35f);
    }

    /* 1. Subtle Perimeter Track */
    HPEN dimPen = CreatePen(PS_SOLID, borderThickness, RGB(26, 34, 48));
    HPEN oldPen = (HPEN)SelectObject(hdc, dimPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc,
              borderThickness / 2,
              borderThickness / 2,
              w - borderThickness / 2,
              h - borderThickness / 2);

    /* 2. 2026 Cyber HUD Corner Brackets (High-Tech Accents) */
    int bracketLen = 24;
    COLORREF bracketCol = animator_glow_color(mainColor, 0.4f * params->breathing_factor);
    HPEN bracketPen = CreatePen(PS_SOLID, borderThickness + 2, bracketCol);
    SelectObject(hdc, bracketPen);

    /* Top-Left */
    MoveToEx(hdc, 0, borderThickness, NULL); LineTo(hdc, bracketLen, borderThickness);
    MoveToEx(hdc, borderThickness, 0, NULL); LineTo(hdc, borderThickness, bracketLen);
    /* Top-Right */
    MoveToEx(hdc, w - bracketLen, borderThickness, NULL); LineTo(hdc, w, borderThickness);
    MoveToEx(hdc, w - borderThickness, 0, NULL); LineTo(hdc, w - borderThickness, bracketLen);
    /* Bottom-Right */
    MoveToEx(hdc, w - bracketLen, h - borderThickness, NULL); LineTo(hdc, w, h - borderThickness);
    MoveToEx(hdc, w - borderThickness, h - bracketLen, NULL); LineTo(hdc, w - borderThickness, h);
    /* Bottom-Left */
    MoveToEx(hdc, 0, h - borderThickness, NULL); LineTo(hdc, bracketLen, h - borderThickness);
    MoveToEx(hdc, borderThickness, h - bracketLen, NULL); LineTo(hdc, borderThickness, h);

    DeleteObject(bracketPen);

    /* 3. Active Elapsed Progress Border */
    int totalLen = 2 * (w + h);
    int filled   = (int)(totalLen * elapsedProgress);

    HPEN brightPen = CreatePen(PS_SOLID, borderThickness, mainColor);
    SelectObject(hdc, brightPen);

    int remain = filled;

    /* Top edge */
    int topLen = min(remain, w);
    if (topLen > 0) {
        MoveToEx(hdc, 0, borderThickness / 2, NULL);
        LineTo(hdc, topLen, borderThickness / 2);
        remain -= topLen;
    }
    /* Right edge */
    if (remain > 0) {
        int rightLen = min(remain, h);
        MoveToEx(hdc, w - borderThickness / 2, 0, NULL);
        LineTo(hdc, w - borderThickness / 2, rightLen);
        remain -= rightLen;
    }
    /* Bottom edge */
    if (remain > 0) {
        int botLen = min(remain, w);
        MoveToEx(hdc, w, h - borderThickness / 2, NULL);
        LineTo(hdc, w - botLen, h - borderThickness / 2);
        remain -= botLen;
    }
    /* Left edge */
    if (remain > 0) {
        int leftLen = min(remain, h);
        MoveToEx(hdc, borderThickness / 2, h, NULL);
        LineTo(hdc, borderThickness / 2, h - leftLen);
    }

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(dimPen);
    DeleteObject(brightPen);
}

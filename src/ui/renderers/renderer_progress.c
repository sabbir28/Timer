#include "ui/renderers/renderer.h"
#include "ui/animation/animator.h"
#include <stdio.h>

static void draw_centered_text(HDC hdc, const RECT* rc, const wchar_t* text, HFONT font, COLORREF color)
{
    HFONT old = (HFONT)SelectObject(hdc, font);
    SetTextColor(hdc, color);
    SetBkMode(hdc, TRANSPARENT);

    RECT r = *rc;
    DrawTextW(hdc, text, -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);

    SelectObject(hdc, old);
}

void render_progress(HDC hdc, const RenderParams* params)
{
    render_minimal(hdc, params);

    if (params->is_live_clock || params->remaining_seconds == 0) return;

    int w = params->client_rect.right - params->client_rect.left;
    int h = params->client_rect.bottom - params->client_rect.top;
    int centerY = h / 2;

    int barW = 320;
    int barH = 8;
    int barX = (w - barW) / 2;
    int barY = centerY + 80;

    /* 1. Track Outline & Background (Pill Track) */
    COLORREF trackOutlineColor = RGB(45, 55, 72);
    HPEN trackPen = CreatePen(PS_SOLID, 1, trackOutlineColor);
    HBRUSH trackBrush = CreateSolidBrush(RGB(20, 26, 38));
    HPEN oldPen = (HPEN)SelectObject(hdc, trackPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, trackBrush);

    RoundRect(hdc, barX - 1, barY - 1, barX + barW + 1, barY + barH + 1, 8, 8);

    /* 2. Determine Dynamic 2026 Color Palette */
    COLORREF barColor = params->palette.accent_color;
    if (params->is_break)
    {
        /* Lush Emerald with breathing modulation */
        barColor = animator_interpolate_color(RGB(16, 185, 129), RGB(52, 211, 153), params->breathing_factor);
    }
    else if (params->remaining_seconds <= 60)
    {
        /* High-urgency pulsating Ruby */
        barColor = animator_interpolate_color(RGB(239, 68, 68), RGB(255, 100, 100), params->pulse_factor);
    }
    else
    {
        /* Focus Electric Cyan with subtle breathing glow */
        barColor = animator_interpolate_color(barColor, RGB(56, 189, 248), params->breathing_factor * 0.4f);
    }

    /* 3. Filled Active Bar with Rounded Cap */
    int fillW = (int)(barW * params->progress);
    if (fillW > 0)
    {
        HBRUSH fillBrush = CreateSolidBrush(barColor);
        HPEN fillPen = CreatePen(PS_SOLID, 1, barColor);
        SelectObject(hdc, fillBrush);
        SelectObject(hdc, fillPen);

        RoundRect(hdc, barX, barY, barX + fillW, barY + barH, 6, 6);

        /* 4. 2026 Travelling Neon Shimmer Sweep */
        if (fillW > 15)
        {
            int shimmerCenter = (int)(params->shimmer_phase * (fillW + 30)) - 15;
            int shimStart = shimmerCenter - 10;
            int shimEnd   = shimmerCenter + 10;
            if (shimStart < 0) shimStart = 0;
            if (shimEnd > fillW) shimEnd = fillW;

            if (shimEnd > shimStart)
            {
                COLORREF shimmerCol = animator_glow_color(barColor, 0.85f);
                HBRUSH shimBrush = CreateSolidBrush(shimmerCol);
                HPEN shimPen = CreatePen(PS_SOLID, 1, shimmerCol);
                SelectObject(hdc, shimBrush);
                SelectObject(hdc, shimPen);

                RoundRect(hdc, barX + shimStart, barY, barX + shimEnd, barY + barH, 4, 4);

                SelectObject(hdc, oldBrush);
                SelectObject(hdc, oldPen);
                DeleteObject(shimBrush);
                DeleteObject(shimPen);
            }
        }

        DeleteObject(fillBrush);
        DeleteObject(fillPen);
    }

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(trackPen);
    DeleteObject(trackBrush);

    /* 5. 2026 Modern Percentage & Mode Badge */
    wchar_t pctBuf[48];
    if (params->is_break)
    {
        swprintf(pctBuf, 48, L"☕ BREAK  •  %d%%", (int)(params->progress * 100.0f + 0.5f));
    }
    else
    {
        swprintf(pctBuf, 48, L"FOCUS  •  %d%% REMAINING", (int)(params->progress * 100.0f + 0.5f));
    }

    RECT pctRc = { 0, barY + 16, w, barY + 44 };
    draw_centered_text(hdc, &pctRc, pctBuf, params->font_small, barColor);
}

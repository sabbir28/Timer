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

void render_minimal(HDC hdc, const RenderParams* params)
{
    int w = params->client_rect.right - params->client_rect.left;
    int h = params->client_rect.bottom - params->client_rect.top;
    int centerY = h / 2;

    /* Background Fill */
    HBRUSH bgBrush = CreateSolidBrush(params->palette.background_color);
    FillRect(hdc, &params->client_rect, bgBrush);
    DeleteObject(bgBrush);

    COLORREF mainColor = params->palette.text_color;
    COLORREF accentColor = params->palette.accent_color;

    /* Low-time animation pulse (< 60s) */
    if (!params->is_live_clock && params->remaining_seconds > 0 && params->remaining_seconds <= 60)
    {
        mainColor = animator_interpolate_color(mainColor, RGB(255, 60, 60), params->pulse_factor);
    }
    else if (params->is_live_clock)
    {
        accentColor = animator_interpolate_color(accentColor, RGB(255, 255, 255), params->pulse_factor * 0.3f);
    }

    if (params->is_live_clock)
    {
        RECT headerRc = { 0, centerY - 110, w, centerY - 80 };
        draw_centered_text(hdc, &headerRc, L"BANGLADESH LIVE CLOCK (BST)", params->font_small, accentColor);

        RECT timeRc = { 0, centerY - 65, w, centerY + 25 };
        draw_centered_text(hdc, &timeRc, params->time_str, params->font_huge, mainColor);

        RECT dateRc = { 0, centerY + 40, w, centerY + 80 };
        draw_centered_text(hdc, &dateRc, params->bst_date_str, params->font_normal, accentColor);

        RECT tzRc = { 0, centerY + 95, w, centerY + 125 };
        draw_centered_text(hdc, &tzRc, L"Barishal / Dhaka (UTC +06:00)", params->font_small, params->palette.text_color);
        return;
    }

    RECT timeRc = { 0, centerY - 80, w, centerY + 20 };
    draw_centered_text(hdc, &timeRc, params->time_str, params->font_huge, mainColor);

    if (params->remaining_seconds == 0)
    {
        RECT msgRc = { 0, centerY + 30, w, centerY + 80 };
        draw_centered_text(hdc, &msgRc, L"TIME'S UP", params->font_normal, RGB(255, 80, 80));
    }
    else if (params->remaining_seconds <= 600 && !params->is_break)
    {
        RECT msgRc = { 0, centerY + 30, w, centerY + 80 };
        draw_centered_text(hdc, &msgRc, L"LAST 10 MINUTES", params->font_normal, accentColor);
    }
    else
    {
        RECT remRc = { 0, centerY + 25, w, centerY + 60 };
        draw_centered_text(hdc, &remRc, L"REMAINING", params->font_small, accentColor);

        RECT dlRc = { 0, centerY + 110, w, centerY + 150 };
        wchar_t dlFull[64];
        swprintf(dlFull, 64, L"DEADLINE  %s", params->deadline_str);
        draw_centered_text(hdc, &dlRc, dlFull, params->font_small, params->palette.text_color);
    }

    if (params->is_break)
    {
        RECT brRc = { 0, centerY - 120, w, centerY - 90 };
        draw_centered_text(hdc, &brRc, L"BREAK", params->font_normal, RGB(100, 200, 140));
    }
}

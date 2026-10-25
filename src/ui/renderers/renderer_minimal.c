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

    /* 1. Deep 2026 Glassmorphic Dark Fill */
    HBRUSH bgBrush = CreateSolidBrush(params->palette.background_color);
    FillRect(hdc, &params->client_rect, bgBrush);
    DeleteObject(bgBrush);

    /* 2. Sleek 2026 Ambient Outer Border with Breathing Glow */
    COLORREF glowBorder = animator_glow_color(params->palette.accent_color, 0.25f + params->breathing_factor * 0.25f);
    HPEN borderPen = CreatePen(PS_SOLID, 1, glowBorder);
    HPEN oldPen = (HPEN)SelectObject(hdc, borderPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
    RoundRect(hdc, 2, 2, w - 2, h - 2, 16, 16);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(borderPen);

    COLORREF mainColor = params->palette.text_color;
    COLORREF accentColor = params->palette.accent_color;

    /* Low-time animation pulse (< 60s) or organic breathing */
    if (!params->is_live_clock && params->remaining_seconds > 0 && params->remaining_seconds <= 60)
    {
        mainColor = animator_interpolate_color(mainColor, RGB(255, 68, 68), params->pulse_factor);
    }
    else if (params->is_live_clock)
    {
        accentColor = animator_glow_color(accentColor, 0.8f + params->breathing_factor * 0.4f);
    }
    else
    {
        /* Subtle 2026 organic text glow breathing */
        mainColor = animator_glow_color(mainColor, 0.9f + params->breathing_factor * 0.2f);
    }

    if (params->is_live_clock)
    {
        RECT headerRc = { 0, centerY - 110, w, centerY - 80 };
        draw_centered_text(hdc, &headerRc, L"🇧🇩 BANGLADESH STANDARD TIME (BST)", params->font_small, accentColor);

        RECT timeRc = { 0, centerY - 65, w, centerY + 25 };
        draw_centered_text(hdc, &timeRc, params->time_str, params->font_huge, mainColor);

        RECT dateRc = { 0, centerY + 40, w, centerY + 80 };
        draw_centered_text(hdc, &dateRc, params->bst_date_str, params->font_normal, accentColor);

        RECT tzRc = { 0, centerY + 95, w, centerY + 125 };
        draw_centered_text(hdc, &tzRc, L"Barishal / Dhaka (UTC +06:00)", params->font_small, params->palette.text_color);
        return;
    }

    /* Break Session Badge Pill */
    if (params->is_break)
    {
        COLORREF breakGlow = animator_glow_color(RGB(52, 211, 153), 0.7f + params->breathing_factor * 0.3f);
        RECT brRc = { 0, centerY - 120, w, centerY - 88 };
        draw_centered_text(hdc, &brRc, L"☕ BREAK SESSION ACTIVE", params->font_normal, breakGlow);
    }

    RECT timeRc = { 0, centerY - 75, w, centerY + 25 };
    draw_centered_text(hdc, &timeRc, params->time_str, params->font_huge, mainColor);

    if (params->remaining_seconds == 0)
    {
        COLORREF alertColor = animator_interpolate_color(RGB(255, 80, 80), RGB(255, 220, 220), params->breathing_factor);
        RECT msgRc = { 0, centerY + 32, w, centerY + 80 };
        draw_centered_text(hdc, &msgRc, L"⚡ SESSION FINISHED", params->font_normal, alertColor);
    }
    else if (params->remaining_seconds <= 600 && !params->is_break)
    {
        COLORREF warnColor = animator_glow_color(RGB(251, 191, 36), 0.8f + params->breathing_factor * 0.3f);
        RECT msgRc = { 0, centerY + 32, w, centerY + 80 };
        draw_centered_text(hdc, &msgRc, L"⏳ FINAL 10 MINUTES", params->font_normal, warnColor);
    }
    else
    {
        RECT remRc = { 0, centerY + 28, w, centerY + 62 };
        draw_centered_text(hdc, &remRc, L"FOCUS FLOW REMAINING", params->font_small, accentColor);

        RECT dlRc = { 0, centerY + 105, w, centerY + 145 };
        wchar_t dlFull[64];
        swprintf(dlFull, 64, L"TARGET  %s", params->deadline_str);
        draw_centered_text(hdc, &dlRc, dlFull, params->font_small, params->palette.text_color);
    }
}

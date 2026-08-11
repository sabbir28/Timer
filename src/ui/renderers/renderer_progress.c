#include "ui/renderers/renderer.h"
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

    int barW = 280;
    int barH = 6;
    int barX = (w - barW) / 2;
    int barY = centerY + 75;

    COLORREF trackColor = RGB(35, 40, 50);
    HBRUSH track = CreateSolidBrush(trackColor);
    RECT trackRc = { barX, barY, barX + barW, barY + barH };
    FillRect(hdc, &trackRc, track);
    DeleteObject(track);

    COLORREF barColor = params->palette.accent_color;

    int fillW = (int)(barW * params->progress);
    HBRUSH fill = CreateSolidBrush(barColor);
    RECT fillRc = { barX, barY, barX + fillW, barY + barH };
    FillRect(hdc, &fillRc, fill);
    DeleteObject(fill);

    wchar_t pctBuf[16];
    swprintf(pctBuf, 16, L"%d%% left", (int)(params->progress * 100.0f + 0.5f));
    RECT pctRc = { 0, barY + 14, w, barY + 40 };
    draw_centered_text(hdc, &pctRc, pctBuf, params->font_small, params->palette.text_color);
}

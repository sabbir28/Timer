#include "ui/renderers/renderer.h"

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

    COLORREF mainColor = params->palette.accent_color;

    HPEN dimPen = CreatePen(PS_SOLID, borderThickness, RGB(35, 40, 50));
    HPEN oldPen = (HPEN)SelectObject(hdc, dimPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    Rectangle(hdc,
              borderThickness / 2,
              borderThickness / 2,
              w - borderThickness / 2,
              h - borderThickness / 2);

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

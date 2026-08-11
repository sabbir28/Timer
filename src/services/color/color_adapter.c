#include "services/color/color_adapter.h"
#include "services/logging/logger.h"
#include <stdio.h>
#include <math.h>

ColorPalette color_adapter_analyze_desktop(HWND hwnd)
{
    ColorPalette palette = {0};
    palette.background_color = RGB(15, 23, 42);
    palette.text_color       = RGB(248, 250, 252);
    palette.accent_color     = RGB(2, 132, 199);
    palette.border_color     = RGB(56, 189, 248);
    palette.is_dark_bg       = true;

    if (!hwnd || !IsWindow(hwnd)) return palette;

    RECT rc;
    GetWindowRect(hwnd, &rc);
    int w = rc.right - rc.left;
    int h = rc.bottom - rc.top;
    if (w <= 0 || h <= 0) return palette;

    HDC hdcDesk = GetDC(NULL);
    if (!hdcDesk) return palette;

    int totalR = 0, totalG = 0, totalB = 0;
    int sampleCount = 0;

    /* Grid sampling 5x5 points */
    int stepX = w / 5;
    int stepY = h / 5;
    if (stepX < 1) stepX = 1;
    if (stepY < 1) stepY = 1;

    for (int y = rc.top + 5; y < rc.bottom - 5; y += stepY)
    {
        for (int x = rc.left + 5; x < rc.right - 5; x += stepX)
        {
            COLORREF px = GetPixel(hdcDesk, x, y);
            if (px != CLR_INVALID)
            {
                totalR += GetRValue(px);
                totalG += GetGValue(px);
                totalB += GetBValue(px);
                sampleCount++;
            }
        }
    }

    ReleaseDC(NULL, hdcDesk);

    if (sampleCount > 0)
    {
        int avgR = totalR / sampleCount;
        int avgG = totalG / sampleCount;
        int avgB = totalB / sampleCount;

        /* Perceived luminance ITU-R BT.709 */
        double lum = (0.2126 * avgR) + (0.7152 * avgG) + (0.0722 * avgB);

        if (lum > 140)
        {
            /* Light Desktop Background -> Smart Dark Card for High Contrast */
            palette.is_dark_bg       = false;
            palette.background_color = RGB(15, 23, 42);    /* Dark slate card */
            palette.text_color       = RGB(248, 250, 252);  /* Crisp white text */
            palette.accent_color     = RGB(14, 165, 233);   /* Electric sky blue */
            palette.border_color     = RGB(56, 189, 248);
        }
        else
        {
            /* Dark Desktop Background -> Vibrant Neon Card */
            palette.is_dark_bg       = true;
            palette.background_color = RGB(0, 0, 0);        /* Deep black overlay */
            palette.text_color       = RGB(220, 245, 255);  /* Ice blue text */
            palette.accent_color     = RGB(0, 240, 255);    /* Neon cyan */
            palette.border_color     = RGB(0, 240, 255);
        }
    }

    return palette;
}

ColorPalette color_adapter_get_preset_palette(int preset_id, COLORREF custom_text, COLORREF custom_bg, COLORREF custom_accent)
{
    ColorPalette palette = {0};

    switch (preset_id)
    {
    case 1: /* Cyberpunk Neon */
        palette.background_color = RGB(15, 10, 25);
        palette.text_color       = RGB(255, 0, 128);
        palette.accent_color     = RGB(0, 240, 255);
        palette.border_color     = RGB(0, 240, 255);
        palette.is_dark_bg       = true;
        break;

    case 2: /* Emerald Barishal */
        palette.background_color = RGB(5, 30, 18);
        palette.text_color       = RGB(240, 255, 245);
        palette.accent_color     = RGB(0, 215, 115);
        palette.border_color     = RGB(255, 80, 80);
        palette.is_dark_bg       = true;
        break;

    case 3: /* Dark Obsidian */
        palette.background_color = RGB(18, 18, 20);
        palette.text_color       = RGB(235, 235, 240);
        palette.accent_color     = RGB(140, 150, 170);
        palette.border_color     = RGB(80, 90, 110);
        palette.is_dark_bg       = true;
        break;

    case 4: /* Solar Gold */
        palette.background_color = RGB(25, 20, 10);
        palette.text_color       = RGB(255, 245, 220);
        palette.accent_color     = RGB(255, 190, 40);
        palette.border_color     = RGB(255, 190, 40);
        palette.is_dark_bg       = true;
        break;

    case 5: /* Custom RGB */
        palette.text_color       = custom_text ? custom_text : RGB(255, 255, 255);
        palette.background_color = custom_bg ? custom_bg : RGB(20, 20, 20);
        palette.accent_color     = custom_accent ? custom_accent : RGB(0, 200, 255);
        palette.border_color     = palette.accent_color;
        palette.is_dark_bg       = true;
        break;

    case 0: /* AI Auto-Palette (Default fallback) */
    default:
        palette.background_color = RGB(15, 23, 42);
        palette.text_color       = RGB(248, 250, 252);
        palette.accent_color     = RGB(2, 132, 199);
        palette.border_color     = RGB(56, 189, 248);
        palette.is_dark_bg       = true;
        break;
    }

    return palette;
}

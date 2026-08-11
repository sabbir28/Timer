#include "ui/windows/overlay_window.h"

void overlay_window_create_fonts(OverlayWindow* window)
{
    if (!window) return;

    window->font_huge = CreateFontW(
        96, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

    window->font_normal = CreateFontW(
        28, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

    window->font_small = CreateFontW(
        18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
}

void overlay_window_destroy_fonts(OverlayWindow* window)
{
    if (!window) return;
    if (window->font_huge)   DeleteObject(window->font_huge);
    if (window->font_normal) DeleteObject(window->font_normal);
    if (window->font_small)  DeleteObject(window->font_small);
    window->font_huge = NULL;
    window->font_normal = NULL;
    window->font_small = NULL;
}

bool overlay_window_create(OverlayWindow* window, HINSTANCE hInst, WNDPROC wndProc)
{
    if (!window || !hInst || !wndProc) return false;
    window->hInst = hInst;

    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = wndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"Timer420Overlay";
    wc.hbrBackground = NULL;

    RegisterClassExW(&wc);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"Timer420Overlay",
        L"Timer420",
        WS_POPUP,
        0, 0, screenW, screenH,
        NULL, NULL, hInst, NULL);

    if (!hwnd) return false;

    window->hwnd = hwnd;

    /* Black background becomes fully transparent */
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    overlay_window_create_fonts(window);
    window->visible = TRUE;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return true;
}

void overlay_window_set_visible(OverlayWindow* window, bool visible)
{
    if (!window || !window->hwnd) return;
    window->visible = visible;
    ShowWindow(window->hwnd, visible ? SW_SHOW : SW_HIDE);
}

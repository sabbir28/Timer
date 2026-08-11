#include "app/controller/app_controller.h"
#include <commctrl.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_WIN95_CLASSES };
    InitCommonControlsEx(&icc);

    AppController* app = app_controller_get_instance();
    if (!app_controller_init(app, hInstance))
    {
        MessageBoxW(NULL, L"Failed to initialize Timer420 application.", L"Timer420", MB_ICONERROR);
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

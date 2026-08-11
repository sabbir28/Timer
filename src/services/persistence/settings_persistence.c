#include "services/persistence/settings_persistence.h"
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

void settings_get_ini_path(wchar_t* buf, size_t buflen)
{
    wchar_t appdata[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, appdata)))
    {
        swprintf(buf, buflen, L"%s\\Timer420", appdata);
        CreateDirectoryW(buf, NULL);
        swprintf(buf, buflen, L"%s\\Timer420\\timer420.ini", appdata);
    }
    else
    {
        wcscpy_s(buf, buflen, L"timer420.ini");
    }
}

bool settings_load(AppSettings* settings, const wchar_t* ini_path)
{
    if (!settings) return false;
    settings_set_defaults(settings);

    wchar_t path[MAX_PATH];
    if (!ini_path)
    {
        settings_get_ini_path(path, MAX_PATH);
        ini_path = path;
    }

    if (GetFileAttributesW(ini_path) == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    settings->work_seconds = GetPrivateProfileIntW(L"Timer", L"WorkSeconds", 7200, ini_path);
    settings->break_seconds = GetPrivateProfileIntW(L"Timer", L"BreakSeconds", 600, ini_path);
    settings->mode = (OverlayMode)GetPrivateProfileIntW(L"Timer", L"OverlayMode", 1, ini_path);
    settings->repeat_enabled = GetPrivateProfileIntW(L"Timer", L"RepeatEnabled", 1, ini_path) != 0;
    settings->repeat_count = GetPrivateProfileIntW(L"Timer", L"RepeatCount", 0, ini_path);

    settings->sound_enabled = GetPrivateProfileIntW(L"Audio", L"SoundEnabled", 1, ini_path) != 0;
    settings->toast_enabled = GetPrivateProfileIntW(L"Notifications", L"ToastEnabled", 1, ini_path) != 0;
    settings->always_on_top = GetPrivateProfileIntW(L"Window", L"AlwaysOnTop", 1, ini_path) != 0;

    settings->live_clock_mode = GetPrivateProfileIntW(L"BangladeshClock", L"LiveClockMode", 0, ini_path) != 0;

    wchar_t offsetBuf[64] = {0};
    GetPrivateProfileStringW(L"BangladeshClock", L"TimeOffsetSeconds", L"0", offsetBuf, 64, ini_path);
    settings->clock_time_offset_seconds = _wtoi64(offsetBuf);

    settings->theme_preset = GetPrivateProfileIntW(L"Theme", L"ThemePreset", 0, ini_path);
    settings->custom_text_color = (COLORREF)GetPrivateProfileIntW(L"Theme", L"CustomTextColor", RGB(255, 255, 255), ini_path);
    settings->custom_bg_color = (COLORREF)GetPrivateProfileIntW(L"Theme", L"CustomBgColor", RGB(15, 23, 42), ini_path);
    settings->custom_accent_color = (COLORREF)GetPrivateProfileIntW(L"Theme", L"CustomAccentColor", RGB(0, 240, 255), ini_path);

    settings_validate(settings);
    return true;
}

bool settings_save(const AppSettings* settings, const wchar_t* ini_path)
{
    if (!settings) return false;

    wchar_t path[MAX_PATH];
    if (!ini_path)
    {
        settings_get_ini_path(path, MAX_PATH);
        ini_path = path;
    }

    wchar_t val[64];

    swprintf(val, 64, L"%u", settings->work_seconds);
    WritePrivateProfileStringW(L"Timer", L"WorkSeconds", val, ini_path);

    swprintf(val, 64, L"%u", settings->break_seconds);
    WritePrivateProfileStringW(L"Timer", L"BreakSeconds", val, ini_path);

    swprintf(val, 64, L"%u", (uint32_t)settings->mode);
    WritePrivateProfileStringW(L"Timer", L"OverlayMode", val, ini_path);

    swprintf(val, 64, L"%d", settings->repeat_enabled ? 1 : 0);
    WritePrivateProfileStringW(L"Timer", L"RepeatEnabled", val, ini_path);

    swprintf(val, 64, L"%u", settings->repeat_count);
    WritePrivateProfileStringW(L"Timer", L"RepeatCount", val, ini_path);

    swprintf(val, 64, L"%d", settings->sound_enabled ? 1 : 0);
    WritePrivateProfileStringW(L"Audio", L"SoundEnabled", val, ini_path);

    swprintf(val, 64, L"%d", settings->toast_enabled ? 1 : 0);
    WritePrivateProfileStringW(L"Notifications", L"ToastEnabled", val, ini_path);

    swprintf(val, 64, L"%d", settings->always_on_top ? 1 : 0);
    WritePrivateProfileStringW(L"Window", L"AlwaysOnTop", val, ini_path);

    swprintf(val, 64, L"%d", settings->live_clock_mode ? 1 : 0);
    WritePrivateProfileStringW(L"BangladeshClock", L"LiveClockMode", val, ini_path);

    swprintf(val, 64, L"%lld", (long long)settings->clock_time_offset_seconds);
    WritePrivateProfileStringW(L"BangladeshClock", L"TimeOffsetSeconds", val, ini_path);

    swprintf(val, 64, L"%d", settings->theme_preset);
    WritePrivateProfileStringW(L"Theme", L"ThemePreset", val, ini_path);

    swprintf(val, 64, L"%u", (uint32_t)settings->custom_text_color);
    WritePrivateProfileStringW(L"Theme", L"CustomTextColor", val, ini_path);

    swprintf(val, 64, L"%u", (uint32_t)settings->custom_bg_color);
    WritePrivateProfileStringW(L"Theme", L"CustomBgColor", val, ini_path);

    swprintf(val, 64, L"%u", (uint32_t)settings->custom_accent_color);
    WritePrivateProfileStringW(L"Theme", L"CustomAccentColor", val, ini_path);

    return true;
}

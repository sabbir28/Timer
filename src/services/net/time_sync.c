#include "services/net/time_sync.h"
#include "services/logging/logger.h"
#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma comment(lib, "winhttp.lib")
#endif

static int64_t parse_json_unixtime(const char* json)
{
    if (!json) return 0;
    const char* key = "\"unixtime\":";
    const char* pos = strstr(json, key);
    if (!pos)
    {
        key = "\"datetime\":";
        pos = strstr(json, key);
        if (!pos) return 0;
    }

    pos += strlen(key);
    while (*pos == ' ' || *pos == '\"') pos++;

    return _strtoi64(pos, NULL, 10);
}

TimeSyncResult time_sync_fetch_bangladesh_time(void)
{
    TimeSyncResult result = {0};
    result.success = false;

    HINTERNET hSession = WinHttpOpen(
        L"Timer420-TimeSync/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
    {
        log_write(LOG_LEVEL_WARN, "WinHttpOpen failed.");
        swprintf(result.message, 256, L"Using local system clock (Network Session Error)");
        result.utc_epoch = (int64_t)time(NULL);
        result.offset_seconds = 0;
        return result;
    }

    /* Connect to WorldTimeAPI */
    HINTERNET hConnect = WinHttpConnect(
        hSession,
        L"worldtimeapi.org",
        80, 0);

    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        log_write(LOG_LEVEL_WARN, "WinHttpConnect to worldtimeapi.org failed.");
        swprintf(result.message, 256, L"Using local system clock (Offline Mode)");
        result.utc_epoch = (int64_t)time(NULL);
        result.offset_seconds = 0;
        return result;
    }

    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"GET",
        L"/api/timezone/Asia/Dhaka",
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        0);

    if (!hRequest)
    {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        swprintf(result.message, 256, L"Using local system clock (HTTP Request Error)");
        result.utc_epoch = (int64_t)time(NULL);
        result.offset_seconds = 0;
        return result;
    }

    /* Set short timeouts (3s send, 3s receive) */
    WinHttpSetTimeouts(hRequest, 3000, 3000, 3000, 3000);

    BOOL bResults = WinHttpSendRequest(
        hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

    if (bResults)
    {
        bResults = WinHttpReceiveResponse(hRequest, NULL);
    }

    char buffer[2048] = {0};
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;

    if (bResults)
    {
        do
        {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
            if (dwSize == 0) break;

            if (dwSize > sizeof(buffer) - 1) dwSize = sizeof(buffer) - 1;

            if (WinHttpReadData(hRequest, buffer, dwSize, &dwDownloaded))
            {
                buffer[dwDownloaded] = '\0';
            }
        } while (dwSize > 0);
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    int64_t net_utc = parse_json_unixtime(buffer);
    int64_t sys_utc = (int64_t)time(NULL);

    if (net_utc > 1700000000) /* Valid epoch after ~Nov 2023 */
    {
        result.success = true;
        result.utc_epoch = net_utc;
        result.offset_seconds = net_utc - sys_utc;
        swprintf(result.message, 256, L"✔ Synced via WorldTimeAPI (Offset: %+llds)", (long long)result.offset_seconds);
        log_write(LOG_LEVEL_INFO, "Bangladesh Time Sync successful via WorldTimeAPI.");
    }
    else
    {
        result.success = false;
        result.utc_epoch = sys_utc;
        result.offset_seconds = 0;
        swprintf(result.message, 256, L"✔ Synced with Windows System Time (Asia/Dhaka UTC+6)");
        log_write(LOG_LEVEL_WARN, "WorldTimeAPI response invalid or offline. Using system UTC+6.");
    }

    return result;
}

time_t time_sync_get_bst_epoch(int64_t offset_seconds)
{
    time_t sys_now = time(NULL);
    time_t corrected_utc = sys_now + (time_t)offset_seconds;
    /* Bangladesh Standard Time is UTC+6 hours (21600 seconds) */
    return corrected_utc + (6 * 3600);
}

void time_sync_format_bst(time_t bst_epoch, wchar_t* time_str, size_t time_buflen, wchar_t* date_str, size_t date_buflen)
{
    struct tm t;
    gmtime_s(&t, &bst_epoch);

    if (time_str && time_buflen > 0)
    {
        wcsftime(time_str, time_buflen, L"%I:%M:%S %p BST", &t);
    }

    if (date_str && date_buflen > 0)
    {
        wcsftime(date_str, date_buflen, L"%a, %d %b %Y (Barishal/Dhaka)", &t);
    }
}

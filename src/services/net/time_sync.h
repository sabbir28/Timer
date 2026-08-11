#ifndef TIMER420_TIME_SYNC_H
#define TIMER420_TIME_SYNC_H

#include <windows.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef struct TimeSyncResult {
    bool success;
    int64_t utc_epoch;
    int64_t offset_seconds;
    wchar_t message[256];
} TimeSyncResult;

/**
 * Synchronizes Bangladesh Standard Time (Asia/Dhaka UTC+6) from internet REST API/WinHTTP.
 */
TimeSyncResult time_sync_fetch_bangladesh_time(void);

/**
 * Returns current Bangladesh Standard Time (epoch timestamp).
 */
time_t time_sync_get_bst_epoch(int64_t offset_seconds);

/**
 * Formats a Bangladesh Standard Time epoch into time string (12-hour AM/PM) and date string.
 */
void time_sync_format_bst(time_t bst_epoch, wchar_t* time_str, size_t time_buflen, wchar_t* date_str, size_t date_buflen);

#endif /* TIMER420_TIME_SYNC_H */

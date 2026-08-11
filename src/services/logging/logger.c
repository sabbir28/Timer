#include "services/logging/logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <windows.h>

static FILE* g_log_file = NULL;
static LogLevel g_min_level = LOG_LEVEL_INFO;

static const char* level_to_string(LogLevel level)
{
    switch (level)
    {
    case LOG_LEVEL_DEBUG: return "DEBUG";
    case LOG_LEVEL_INFO:  return "INFO";
    case LOG_LEVEL_WARN:  return "WARN";
    case LOG_LEVEL_ERROR: return "ERROR";
    default:              return "UNKNOWN";
    }
}

bool logger_init(const char* filepath, LogLevel min_level)
{
    g_min_level = min_level;
    if (filepath && filepath[0] != '\0')
    {
        g_log_file = fopen(filepath, "a");
        if (!g_log_file) return false;
    }
    return true;
}

void log_write(LogLevel level, const char* format, ...)
{
    if (level < g_min_level) return;

    time_t now = time(NULL);
    struct tm t;
    localtime_s(&t, &now);

    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", &t);

    char msg[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);

    char log_line[1200];
    snprintf(log_line, sizeof(log_line), "[%s] [%-5s] %s\n", time_str, level_to_string(level), msg);

    OutputDebugStringA(log_line);

    if (g_log_file)
    {
        fputs(log_line, g_log_file);
        fflush(g_log_file);
    }
}

void logger_close(void)
{
    if (g_log_file)
    {
        fclose(g_log_file);
        g_log_file = NULL;
    }
}

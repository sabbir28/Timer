#ifndef TIMER420_LOGGER_H
#define TIMER420_LOGGER_H

#include <stdbool.h>

typedef enum LogLevel {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

/**
 * Initializes the logger service with log file path and minimum severity level.
 */
bool logger_init(const char* filepath, LogLevel min_level);

/**
 * Writes a formatted log entry.
 */
void log_write(LogLevel level, const char* format, ...);

/**
 * Flushes buffers and closes log file handles.
 */
void logger_close(void);

#endif /* TIMER420_LOGGER_H */

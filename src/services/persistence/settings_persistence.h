#ifndef TIMER420_SETTINGS_PERSISTENCE_H
#define TIMER420_SETTINGS_PERSISTENCE_H

#include "core/config/settings.h"

/**
 * Gets the standard configuration file path under AppData/Timer420/timer420.ini.
 */
void settings_get_ini_path(wchar_t* buf, size_t buflen);

/**
 * Loads settings from INI file. Falls back to defaults if file missing or corrupt.
 */
bool settings_load(AppSettings* settings, const wchar_t* ini_path);

/**
 * Saves settings to INI file.
 */
bool settings_save(const AppSettings* settings, const wchar_t* ini_path);

#endif /* TIMER420_SETTINGS_PERSISTENCE_H */

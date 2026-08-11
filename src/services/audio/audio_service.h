#ifndef TIMER420_AUDIO_SERVICE_H
#define TIMER420_AUDIO_SERVICE_H

#include <stdbool.h>
#include <wchar.h>

/**
 * Plays default completion sound or custom sound file asynchronously.
 */
void audio_play_completion(const wchar_t* custom_sound_path);

#endif /* TIMER420_AUDIO_SERVICE_H */

#include "services/audio/audio_service.h"
#include <windows.h>
#include <mmsystem.h>

void audio_play_completion(const wchar_t* custom_sound_path)
{
    if (custom_sound_path && custom_sound_path[0] != L'\0')
    {
        if (PlaySoundW(custom_sound_path, NULL, SND_FILENAME | SND_ASYNC))
        {
            return;
        }
    }

    /* Fallback to system default notification sound */
    if (!PlaySoundW(L"SystemNotification", NULL, SND_ALIAS | SND_ASYNC))
    {
        MessageBeep(MB_ICONASTERISK);
    }
}

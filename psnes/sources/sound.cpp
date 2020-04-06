/*****************************************************************************\
     Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
                This file is licensed under the Snes9x License.
   For further information, consult the LICENSE file in the root directory.
\*****************************************************************************/

#include <SDL2/SDL.h>

#include "snes9x.h"
#include "apu/apu.h"
#include "apu/resampler.h"
#include "sound.h"

#ifdef __PSP2__

#include <psp2/kernel/threadmgr.h>

#define usleep sceKernelDelayThread
#endif

static Resampler buffer;
static int16_t temp[512];
static SDL_mutex *mutex;

static void sdl_audio_callback(void *userdata, Uint8 *stream, int len) {

    SDL_LockMutex(mutex);
    if (buffer.avail() >= len >> 1)
        buffer.read((int16_t *) stream, len >> 1);
    SDL_UnlockMutex(mutex);
}

static void samples_available(void *data) {

    int snes_samples_available = S9xGetSampleCount();
    S9xMixSamples((uint8 *) temp, snes_samples_available);

    if (Settings.SoundSync && !Settings.TurboMode && !Settings.Mute) {
        SDL_LockMutex(mutex);
        int samples = buffer.space_empty();
        SDL_UnlockMutex(mutex);

        while (samples < snes_samples_available) {
            usleep(100);
            SDL_LockMutex(mutex);
            samples = buffer.space_empty();
            SDL_UnlockMutex(mutex);
        }
    }

    SDL_LockMutex(mutex);
    buffer.push(temp, snes_samples_available);
    SDL_UnlockMutex(mutex);
}

void
S9xPortSoundInit() {

    mutex = SDL_CreateMutex();

    SDL_InitSubSystem(SDL_INIT_AUDIO);
    SDL_PauseAudio(1);

    S9xInitSound(0);
    S9xSetSoundMute(false);
}

void
S9xPortSoundDeinit() {

    S9xSoundStop();

    SDL_PauseAudio(1);
    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    SDL_DestroyMutex(mutex);
}

void
S9xSoundStart() {

    if (Settings.Mute == FALSE) {
        SDL_PauseAudio(0);
    }
}

void
S9xSoundStop() {

    SDL_PauseAudio(1);
}

bool8
S9xOpenSoundDevice() {
    if (Settings.Mute == TRUE)
        return FALSE;

    SDL_AudioSpec spec = {};
    spec.freq = Settings.SoundPlaybackRate;
    spec.channels = 2;
    spec.format = AUDIO_S16SYS;
    spec.samples = (sound_buffer_size * spec.freq / 1000) >> 2;
    spec.callback = sdl_audio_callback;

    printf("SDL sound driver initializing...\n");
    printf("    --> (Frequency: %dhz, Latency: %dms)...",
           spec.freq, (spec.samples * 1000 / spec.freq));

    if (SDL_OpenAudio(&spec, nullptr) < 0) {
        printf("Failed\n");
        return FALSE;
    }

    printf("OK\n");

    buffer.resize(sound_buffer_size * spec.freq / 500);
    buffer.time_ratio(1.0);

    S9xSetSamplesAvailableCallback(samples_available, nullptr);

    return TRUE;

}

void
S9xToggleSoundChannel(int c) {
    static int sound_switch = 255;

    if (c == 8)
        sound_switch = 255;
    else
        sound_switch ^= 1 << c;

    S9xSetSoundControl(sound_switch);
}

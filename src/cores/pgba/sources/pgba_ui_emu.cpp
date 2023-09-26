//
// Created by cpasjuste on 25/09/23.
//

#include "skeleton/pemu.h"
#include "pgba_ui_emu.h"
#include "mgba/core/blip_buf.h"
extern "C" {
#include "mgba-util/vfs.h"
#include "mgba/core/log.h"
}

#define VIDEO_WIDTH_MAX  240
#define VIDEO_HEIGHT_MAX 160
#define SAMPLE_RATE 32768

mCore *s_core = nullptr;
static int16_t *audioSampleBuffer = nullptr;
static float audioSamplesPerFrameAvg = 0;
static size_t audioSampleBufferSize = 0;
#define SAMPLES_PER_FRAME_MOVING_AVG_ALPHA (1.0f / 180.0f)

PGBAUiEmu::PGBAUiEmu(UiMain *ui) : UiEmu(ui) {
    printf("PGENUiEmu()\n");
}

int PGBAUiEmu::load(const ss_api::Game &game) {
    currentGame = game;
    getUi()->getUiProgressBox()->setTitle(game.name);
    getUi()->getUiProgressBox()->setMessage("Please wait...");
    getUi()->getUiProgressBox()->setProgress(0);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Visible);
    getUi()->getUiProgressBox()->setLayer(1000);
    getUi()->flip();

    // mgba int
    auto path = game.romsPath + game.path;
    auto rom = VFileOpen(path.c_str(), O_RDONLY);
    s_core = mCoreFindVF(rom);
    if (!s_core) {
        rom->close(rom);
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "INVALID FILE OR MISSING BIOS...", "OK");
        return -1;
    }
    mCoreInitConfig(s_core, nullptr);
    s_core->init(s_core);

    // video output
    uint8_t *buffer;
    int pitch;
    addVideo(&buffer, &pitch, {VIDEO_WIDTH_MAX, VIDEO_HEIGHT_MAX});
    s_core->setVideoBuffer(s_core, reinterpret_cast<color_t *>(buffer), VIDEO_WIDTH_MAX);

    // audio output
    auto audioSamplesPerFrame = (size_t) ((float) SAMPLE_RATE * (float) s_core->frameCycles(s_core) /
                                          (float) s_core->frequency(s_core));
    audioSampleBufferSize = audioSamplesPerFrame * 2;
    audioSampleBuffer = static_cast<int16_t *>(malloc(audioSampleBufferSize * sizeof(int16_t)));
    audioSamplesPerFrameAvg = (float) audioSamplesPerFrame;
    size_t internalAudioBufferSize = audioSamplesPerFrame * 2;
    if (internalAudioBufferSize > 0x4000) {
        internalAudioBufferSize = 0x4000;
    }
    s_core->setAudioBufferSize(s_core, internalAudioBufferSize);
    blip_set_rates(s_core->getAudioChannel(s_core, 0), s_core->frequency(s_core), SAMPLE_RATE);
    blip_set_rates(s_core->getAudioChannel(s_core, 1), s_core->frequency(s_core), SAMPLE_RATE);
    addAudio(SAMPLE_RATE, Audio::toSamples(SAMPLE_RATE, 60));

    // set core options
    struct mCoreOptions opts = {
            .useBios = true,
            .volume = 0x100,
    };
    mCoreConfigLoadDefaults(&s_core->config, &opts);
    mCoreLoadConfig(s_core);

    // load rom
    if (!s_core->loadROM(s_core, rom)) {
        stop();
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "INVALID FILE OR MISSING BIOS...", "OK");
        return -1;
    }

    // load bios if any
    path = getUi()->getIo()->getDataPath() + "bios/gba_bios.bin";
    if (getUi()->getIo()->exist(path)) {
        struct VFile *bios = VFileOpen(path.c_str(), O_RDONLY);
        if (bios) {
            printf("PGBAUiEmu::load: loading bios: %s\n", path.c_str());
            s_core->loadBIOS(s_core, bios, 0);
        }
    } else {
        mCoreConfigSetIntValue(&s_core->config, "useBios", 0);
        s_core->reloadConfigOption(s_core, "useBios", nullptr);
    }

    getUi()->getUiProgressBox()->setProgress(1);
    getUi()->flip();
    getUi()->delay(500);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);

    // let's go
    s_core->reset(s_core);

    return UiEmu::load(game);
}

void PGBAUiEmu::stop() {
    if (s_core) {
        mCoreConfigDeinit(&s_core->config);
        s_core->deinit(s_core);
    }

    if (audioSampleBuffer) {
        free(audioSampleBuffer);
        audioSampleBuffer = nullptr;
    }

    UiEmu::stop();
}

void PGBAUiEmu::onUpdate() {
    if (!isPaused()) {
        // inputs
#if 0
        for (int i = 0; i < PLAYER_MAX; i++) {
            unsigned int buttons = getUi()->getInput()->getButtons(i);
        }
#endif

        // frame emulation
        s_core->runFrame(s_core);

        // video
        //unsigned width, height;
        //s_core->currentVideoSize(s_core, &width, &height);
        //printf("%i %i\n", width, height);
        video->unlock();

        // audio
        if (s_core->platform(s_core) == mPLATFORM_GBA) {
            if (!audioSampleBuffer) return;

            blip_t *audioChannelLeft = s_core->getAudioChannel(s_core, 0);
            blip_t *audioChannelRight = s_core->getAudioChannel(s_core, 1);
            int samplesAvail = blip_samples_avail(audioChannelLeft);
            if (samplesAvail > 0) {
                audioSamplesPerFrameAvg = (SAMPLES_PER_FRAME_MOVING_AVG_ALPHA * (float) samplesAvail) +
                                          ((1.0f - SAMPLES_PER_FRAME_MOVING_AVG_ALPHA) * audioSamplesPerFrameAvg);
                auto samplesToRead = (size_t) audioSamplesPerFrameAvg;
                if (audioSampleBufferSize < samplesToRead * 2) {
                    audioSampleBufferSize = samplesToRead * 2;
                    audioSampleBuffer = (int16_t *) realloc(audioSampleBuffer, audioSampleBufferSize * sizeof(int16_t));
                    if (!audioSampleBuffer) return;
                }
                int produced = blip_read_samples(audioChannelLeft, audioSampleBuffer, (int) samplesToRead, true);
                blip_read_samples(audioChannelRight, audioSampleBuffer + 1, (int) samplesToRead, true);
                if (produced > 0) {
                    audio->play(audioSampleBuffer, produced);
                }
            }
        }
    }

    return UiEmu::onUpdate();
}

//
// Created by cpasjuste on 25/09/23.
//

#include "skeleton/pemu.h"
#include "pgba_ui_emu.h"
extern "C" {
#include "mgba-util/vfs.h"
#include "mgba/core/log.h"
#include "mgba/core/blip_buf.h"
#include "mgba/core/serialize.h"
#include "mgba/internal/gba/input.h"
}

#define VIDEO_WIDTH_MAX  240
#define VIDEO_HEIGHT_MAX 160
#define SAMPLE_RATE 32768

mCore *s_core = nullptr;
static VDir *s_vDir;
static VFile *s_vFile;
static int16_t *audioSampleBuffer = nullptr;
static float audioSamplesPerFrameAvg = 0;
static size_t audioSampleBufferSize = 0;
#define SAMPLES_PER_FRAME_MOVING_AVG_ALPHA (1.0f / 180.0f)

#define PEMU_INPUT_BINDING 0x47434E31

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
    s_core = mCoreFind(path.c_str());
    if (!s_core) {
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "INVALID ROM FILE", "OK");
        return -1;
    }

    mCoreInitConfig(s_core, nullptr);
    s_core->init(s_core);

    // inputs
    mInputMapInit(&s_core->inputMap, &GBAInputInfo);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::A), GBA_KEY_A);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::B), GBA_KEY_B);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::LB), GBA_KEY_L);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::RB), GBA_KEY_R);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::Start), GBA_KEY_START);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::Select), GBA_KEY_SELECT);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::Up), GBA_KEY_UP);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::Down), GBA_KEY_DOWN);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::Left), GBA_KEY_LEFT);
    mInputBindKey(&s_core->inputMap, PEMU_INPUT_BINDING, __builtin_ctz(Input::Button::Right), GBA_KEY_RIGHT);
    mInputMapLoad(&s_core->inputMap, PEMU_INPUT_BINDING, mCoreConfigGetInput(&s_core->config));

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

    // core options
    //auto dataPath = getUi()->getIo()->getDataPath();
    //char savePath[512];
    //strncpy(savePath, "/home/cpasjuste/dev/multi/pemu/cmake-build-debug/src/cores/pgba/saves", 511);
    mCoreOptions core_options = {
            .useBios = true,
            .logLevel = mLOG_WARN | mLOG_ERROR | mLOG_FATAL,
            .rewindEnable = false,
            .rewindBufferCapacity = 600,
            .rewindBufferInterval = 1,
            .audioBuffers = 1024,
            //.savestatePath = savePath,
            //.screenshotPath = (char *) std::string(dataPath + "screenshots").c_str(),
            //.patchPath = (char *) std::string(dataPath + "patches").c_str(),
            //.cheatsPath = (char *) std::string(dataPath + "cheats").c_str(),
            .volume = 0x100,
            .videoSync = false,
            .audioSync = true
    };
    mCoreConfigLoadDefaults(&s_core->config, &core_options);
    mCoreLoadConfig(s_core);

    // set custom paths
    auto dataPath = getUi()->getIo()->getDataPath();
    std::string savePath = dataPath + "saves";
    std::string patchPath = dataPath + "patches";
    std::string cheatPath = dataPath + "cheats";
    mCoreOptions opts = {
            .savegamePath =  (char *) savePath.c_str(),
            .patchPath = (char *) patchPath.c_str(),
            .cheatsPath = (char *) cheatPath.c_str()
    };
    mDirectorySetMapOptions(&s_core->dirs, &opts);

    // load rom
    if (!mCoreLoadFile(s_core, path.c_str())) {
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "INVALID ROM FILE", "OK");
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

    // load save if any
    mCoreAutoloadSave(s_core);
    mCoreAutoloadCheats(s_core);
    mCoreAutoloadPatch(s_core);

#if 0
    int autoload = false;
    mCoreConfigGetIntValue(&s_core->config, "autoload", &autoload);
    if (autoload) {
        mCoreLoadState(s_core, 0, SAVESTATE_RTC);
    }
#endif

    return UiEmu::load(game);
}

void PGBAUiEmu::stop() {
    if (s_core) {
#if 0
        int autosave = false;
        mCoreConfigGetIntValue(&s_core->config, "autosave", &autosave);
        if (autosave) {
            mCoreSaveState(s_core, 0, SAVESTATE_SAVEDATA | SAVESTATE_RTC | SAVESTATE_METADATA);
        }
#endif
        mInputMapDeinit(&s_core->inputMap);
        mCoreConfigDeinit(&s_core->config);
        s_core->unloadROM(s_core);
        s_core->deinit(s_core);
    }

    if (s_vDir) {
        s_vDir->close(s_vDir);
        s_vDir = nullptr;
    } else if (s_vFile) {
        s_vFile->close(s_vFile);
        s_vFile = nullptr;
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
        auto buttons = getUi()->getInput()->getButtons(0);
        auto keys = mInputMapKeyBits(&s_core->inputMap, PEMU_INPUT_BINDING, buttons, 0);
        s_core->setKeys(s_core, keys);

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

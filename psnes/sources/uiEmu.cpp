//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "uiEmu.h"

#include <snes9x.h>
#include <memmap.h>
#include <apu/apu.h>
#include <controls.h>
#include <conffile.h>
#include <display.h>
#include <sys/stat.h>
#include <cheats.h>

#ifdef __PSP2__
#include <psp2/io/dirent.h>
#include <psp2/kernel/threadmgr.h>
#define usleep sceKernelDelayThread
#endif

using namespace c2d;
using namespace c2dui;

static UiMain *m_ui;

static Option *optionAudioSync = nullptr;

static void *audio_buffer = nullptr;

static const char *s9x_base_dir = nullptr;

static char default_dir[PATH_MAX + 1];

static const char dirNames[13][32] = {
        "",             // DEFAULT_DIR
        "",             // HOME_DIR
        "",             // ROMFILENAME_DIR
        "roms",          // ROM_DIR
        "sram",         // SRAM_DIR
        "saves",        // SNAPSHOT_DIR
        "screenshots",  // SCREENSHOT_DIR
        "spc",          // SPC_DIR
        "cheat",        // CHEAT_DIR
        "patch",        // PATCH_DIR
        "bios",         // BIOS_DIR
        "log",          // LOG_DIR
        ""
};

static int S9xCreateDirectory();

static void S9xEndScreenRefreshCallback(void *);

std::string getButtonId(int player, const std::string &name) {
    return "Joypad" + std::to_string(player) + " " + name;
}

PSNESUiEmu::PSNESUiEmu(UiMain *ui) : UiEmu(ui) {
    printf("PSNESUIEmu()\n");
    m_ui = ui;
}

int PSNESUiEmu::load(const ss_api::Game &game) {
    pMain->getUiProgressBox()->setTitle(game.name);
    pMain->getUiProgressBox()->setMessage("Please wait...");
    pMain->getUiProgressBox()->setProgress(0);
    pMain->getUiProgressBox()->setVisibility(Visibility::Visible);
    pMain->getUiProgressBox()->setLayer(1000);
    pMain->flip();

    strncpy(default_dir, pMain->getIo()->getDataPath().c_str(), PATH_MAX);
    s9x_base_dir = default_dir;

    memset(&Settings, 0, sizeof(Settings));
    S9xLoadConfigFiles(nullptr, 0);

    // override S9xLoadConfigFiles
    Settings.MouseMaster = FALSE;
    Settings.SuperScopeMaster = FALSE;
    Settings.JustifierMaster = FALSE;
    Settings.MultiPlayer5Master = FALSE;
    Settings.FrameTimePAL = 20000;
    Settings.FrameTimeNTSC = 16667;
    // audio
    Settings.SixteenBitSound = TRUE;
    Settings.Stereo = TRUE;
    Settings.SoundSync = TRUE;
    Settings.SoundInputRate = 31950;
    Settings.SoundPlaybackRate = 48000;

    // audio
    Settings.Transparency =
            pMain->getConfig()->get(Option::ROM_PSNES_TRANSPARENCY, true)->getIndex();
    Settings.AutoDisplayMessages =
            pMain->getConfig()->get(Option::ROM_PSNES_DISPLAY_MESSAGES, true)->getIndex();
    Settings.InitialInfoStringTimeout = 120;
    Settings.HDMATimingHack = 100;
    Settings.BlockInvalidVRAMAccessMaster =
            pMain->getConfig()->get(Option::ROM_PSNES_BLOCK_VRAM, true)->getIndex();
    Settings.StopEmulation = TRUE;
    Settings.WrongMovieStateProtection = TRUE;
    Settings.DumpStreamsMaxFrames = -1;
    Settings.StretchScreenshots = 1;
    Settings.SnapshotScreenshots = FALSE;
    Settings.DontSaveOopsSnapshot = TRUE;
    Settings.FastSavestates = TRUE;
    Settings.SeparateEchoBuffer = FALSE;

    int skipFramesCfg = pMain->getConfig()->get(Option::ROM_PSNES_FRAMESKIP, true)->getIndex();
    if (skipFramesCfg == 0) {
        Settings.SkipFrames = 0;
    } else if (skipFramesCfg == 1) {
        Settings.SkipFrames = AUTO_FRAMERATE;
    } else {
        Settings.SkipFrames = skipFramesCfg - 1;
    }
    printf("Settings.SkipFrames: %i\n", Settings.SkipFrames);
    Settings.TurboMode = pMain->getConfig()->get(Option::ROM_PSNES_TURBO_MODE, true)->getIndex();
    Settings.TurboSkipFrames = pMain->getConfig()->get(Option::ROM_PSNES_TURBO_FRAMESKIP,
                                                       true)->getIndex();
    Settings.CartAName[0] = 0;
    Settings.CartBName[0] = 0;

    CPU.Flags = 0;

    S9xCreateDirectory();

    if (!Memory.Init() || !S9xInitAPU()) {
        Memory.Deinit();
        S9xDeinitAPU();
        printf("Could not initialize Snes9x Memory.\n");
        pMain->getUiProgressBox()->setVisibility(Visibility::Hidden);
        stop();
        return -1;
    }

    S9xInitSound(32);
    S9xSetSoundMute(FALSE);
    S9xSetSamplesAvailableCallback(nullptr, nullptr);

    //getButtonId
    S9xUnmapAllControls();
    for (int i = 0; i < 4; i++) {
        S9xSetController(i, CTL_JOYPAD, (int8) i, 0, 0, 0);
        S9xMapButton(0 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "Up").c_str()), false);
        S9xMapButton(1 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "Down").c_str()), false);
        S9xMapButton(2 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "Left").c_str()), false);
        S9xMapButton(3 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "Right").c_str()), false);
        S9xMapButton(4 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "A").c_str()), false);
        S9xMapButton(5 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "B").c_str()), false);
        S9xMapButton(6 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "X").c_str()), false);
        S9xMapButton(7 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "Y").c_str()), false);
        S9xMapButton(8 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "L").c_str()), false);
        S9xMapButton(9 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "R").c_str()), false);
        S9xMapButton(10 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "Start").c_str()), false);
        S9xMapButton(11 + (i * 12), S9xGetCommandT(getButtonId(i + 1, "Select").c_str()), false);
    }
    S9xReportControllers();

    uint32 saved_flags = CPU.Flags;

    std::string fullPath = game.romsPath + game.path;
    printf("Memory.LoadROM: %s\n", fullPath.c_str());
    if (!Memory.LoadROM(fullPath.c_str())) {
        printf("Could not open ROM: %s\n", fullPath.c_str());
        pMain->getUiProgressBox()->setVisibility(Visibility::Hidden);
        pMain->getUiMessageBox()->show("ERROR", "INVALID ROM", "OK");
        stop();
        return -1;
    }

    Memory.LoadSRAM(S9xGetFilename(".srm", SRAM_DIR));

    Settings.ApplyCheats = pMain->getConfig()->get(Option::ROM_PSNES_CHEATS, true)->getIndex() == 1;
    S9xDeleteCheats();
    S9xCheatsEnable();
    if (Settings.ApplyCheats) {
        printf("S9xLoadCheatFile(%s)\n", S9xGetFilename(".cht", CHEAT_DIR));
        S9xLoadCheatFile(S9xGetFilename(".cht", CHEAT_DIR));
    }

    CPU.Flags = saved_flags;
    Settings.StopEmulation = FALSE;

    // audio
    int samples = Audio::toSamples((int) Settings.SoundPlaybackRate,
                                   (float) Memory.ROMFramesPerSecond);
    addAudio((int) Settings.SoundPlaybackRate, samples * 2);
    audio_buffer = malloc(getAudio()->getSamplesSize() * getAudio()->getChannels() * 5);
    optionAudioSync = pMain->getConfig()->get(Option::Id::ROM_AUDIO_SYNC, true);

    // video
    S9xGraphicsInit();
    S9xSetEndScreenRefreshCallback(S9xEndScreenRefreshCallback, getAudio());

    addVideo((uint8_t **) &GFX.Screen, (int *) &GFX.Pitch, {MAX_SNES_WIDTH, MAX_SNES_HEIGHT});
    targetFps = (float) Memory.ROMFramesPerSecond;

    pMain->getUiProgressBox()->setProgress(1);
    pMain->flip();
    pMain->delay(500);
    pMain->getUiProgressBox()->setVisibility(Visibility::Hidden);

    return UiEmu::load(game);
}

void PSNESUiEmu::stop() {
    Settings.StopEmulation = TRUE;

    Memory.SaveSRAM(S9xGetFilename(".srm", SRAM_DIR));

    S9xUnmapAllControls();
    S9xGraphicsDeinit();
    Memory.Deinit();
    S9xDeinitAPU();

    if (audio_buffer) {
        free(audio_buffer);
    }

    UiEmu::stop();
}

bool PSNESUiEmu::onInput(c2d::Input::Player *players) {
    return UiEmu::onInput(players);
}

void PSNESUiEmu::onUpdate() {
    UiEmu::onUpdate();

    if (isVisible() && !isPaused()) {
        auto players = pMain->getInput()->getPlayers();

        // update snes9x buttons
        for (uint32 i = 0; i < 4; i++) {
            unsigned int buttons = players[i].buttons;
            S9xReportButton(0 + (i * 12), (buttons & Input::Button::Up) > 0);
            S9xReportButton(1 + (i * 12), (buttons & Input::Button::Down) > 0);
            S9xReportButton(2 + (i * 12), (buttons & Input::Button::Left) > 0);
            S9xReportButton(3 + (i * 12), (buttons & Input::Button::Right) > 0);
            S9xReportButton(4 + (i * 12), (buttons & Input::Button::A) > 0);
            S9xReportButton(5 + (i * 12), (buttons & Input::Button::B) > 0);
            S9xReportButton(6 + (i * 12), (buttons & Input::Button::X) > 0);
            S9xReportButton(7 + (i * 12), (buttons & Input::Button::Y) > 0);
            S9xReportButton(8 + (i * 12), (buttons & Input::Button::LT) > 0);
            S9xReportButton(9 + (i * 12), (buttons & Input::Button::RT) > 0);
            S9xReportButton(10 + (i * 12), (buttons & Input::Button::Start) > 0);
            S9xReportButton(11 + (i * 12), (buttons & Input::Button::Select) > 0);
        }

        S9xMainLoop();
    }
}

void PSNESUiEmu::pause() {
    S9xSetSoundMute(TRUE);
    S9xClearSamples();
    UiEmu::pause();
}

void PSNESUiEmu::resume() {
    S9xSetSoundMute(FALSE);
    UiEmu::resume();
}

///////////////////////////////////////////////////////////////////////////////
// Functions called by snes9x below
///////////////////////////////////////////////////////////////////////////////

static void S9xEndScreenRefreshCallback(void *data) {
    if (Settings.Mute) {
        S9xClearSamples();
        return;
    }

    auto audio = (Audio *) data;
    int samples = S9xGetSampleCount();
    S9xMixSamples((uint8 *) audio_buffer, samples);
#if 0
    int queued = audio->getSampleBufferQueued();
    int capacity = audio->getSampleBufferCapacity();
    if (samples + queued > capacity) {
        printf("WARNING: samples: %i, queued: %i, capacity: %i (fps: %i)\n",
               samples, queued, capacity, Memory.ROMFramesPerSecond);
    }
#endif
    Audio::SyncMode mode = Memory.ROMFramesPerSecond < 60 || optionAudioSync->getValueBool() ?
                           Audio::SyncMode::Safe : Audio::SyncMode::None;
    audio->play(audio_buffer, samples >> 1, mode);
}

bool8 S9xInitUpdate() {
    return TRUE;
}

bool8 S9xDeinitUpdate(int width, int height) {
    //printf("S9xDeinitUpdate(%i, %i\n", width, height);
    C2DUIVideo *video = m_ui->getUiEmu()->getVideo();
    if (video) {
        auto rect = video->getTextureRect();
        if (rect.width != width || rect.height != height) {
            video->setSize((float) width, (float) height);
            video->setTextureRect({0, 0, width, height});
            video->updateScaling();
        }
        video->unlock();
    }

    return TRUE;
}

bool8 S9xContinueUpdate(int width, int height) {
    S9xDeinitUpdate(width, height);
    return TRUE;
}

static int S9xCreateDirectory() {
    if (strlen(s9x_base_dir) + 1 + sizeof(dirNames[0]) > PATH_MAX + 1)
        return (-1);

    m_ui->getIo()->create(s9x_base_dir);

    for (int i = 0; i < LAST_DIR; i++) {
        if (dirNames[i][0]) {
            char s[PATH_MAX + 1];
            snprintf(s, PATH_MAX + 1, "%s%s%s", s9x_base_dir, SLASH_STR, dirNames[i]);
            m_ui->getIo()->create(s);
        }
    }

    return (0);
}

const char *S9xGetDirectory(s9x_getdirtype dirtype) {
    static char s[PATH_MAX + 1];

    if (dirNames[dirtype][0])
        snprintf(s, PATH_MAX + 1, "%s%s%s", s9x_base_dir, SLASH_STR, dirNames[dirtype]);
    else {
        switch (dirtype) {

            case DEFAULT_DIR:
                strncpy(s, s9x_base_dir, PATH_MAX + 1);
                s[PATH_MAX] = 0;
                break;

            case HOME_DIR:
                strncpy(s, s9x_base_dir, PATH_MAX + 1);
                s[PATH_MAX] = 0;
                break;

            case ROMFILENAME_DIR:
                strncpy(s, Memory.ROMFilename, PATH_MAX + 1);
                s[PATH_MAX] = 0;

                for (int i = (int) strlen(s); i >= 0; i--) {
                    if (s[i] == SLASH_CHAR) {
                        s[i] = 0;
                        break;
                    }
                }
                break;

            default:
                s[0] = 0;
                break;
        }
    }

    return (s);
}

const char *S9xGetFilename(const char *ex, s9x_getdirtype dirtype) {
    static char s[PATH_MAX + 1];
    char drive[_MAX_DRIVE + 1], dir[_MAX_DIR + 1], fname[_MAX_FNAME + 1], ext[_MAX_EXT + 1];

    _splitpath(Memory.ROMFilename, drive, dir, fname, ext);
    snprintf(s, PATH_MAX + 1, "%s%s%s%s", S9xGetDirectory(dirtype), SLASH_STR, fname, ex);

    return (s);
}

const char *S9xGetFilenameInc(const char *ex, s9x_getdirtype dirtype) {
    static char s[PATH_MAX + 1];
    char drive[_MAX_DRIVE + 1], dir[_MAX_DIR + 1], fname[_MAX_FNAME + 1], ext[_MAX_EXT + 1];

    unsigned int i = 0;
    const char *d;
    struct stat buf = {};

    _splitpath(Memory.ROMFilename, drive, dir, fname, ext);
    d = S9xGetDirectory(dirtype);

    do
        snprintf(s, PATH_MAX + 1, "%s%s%s.%03d%s", d, SLASH_STR, fname, i++, ex);
    while (stat(s, &buf) == 0 && i < 1000);

    return (s);

}

const char *S9xBasename(const char *f) {
    const char *p;

    if ((p = strrchr(f, '/')) != NULL || (p = strrchr(f, '\\')) != NULL)
        return (p + 1);

    return (f);
}

bool8 S9xOpenSnapshotFile(const char *filename, bool8 read_only, STREAM*file) {
    if ((*file = OPEN_STREAM(filename, read_only ? "rb" : "wb"))) return (TRUE);
    return (FALSE);
}

void S9xCloseSnapshotFile(STREAM file) {
    CLOSE_STREAM(file);
}

void S9xAutoSaveSRAM() {
    Memory.SaveSRAM(S9xGetFilename(".srm", SRAM_DIR));
}

void S9xExit() {
    m_ui->getUiEmu()->stop();
}

void S9xMessage(int type, int number, const char *message) {
    const int max = 36 * 3;
    static char buffer[max + 1];
    printf("[%i][%i]: %s\n", type, number, message);
    strncpy(buffer, message, max + 1);
    buffer[max] = 0;
    S9xSetInfoString(buffer);
}

const char *S9xStringInput(const char *message) { return nullptr; }

void S9xSetPalette() {}

bool8 S9xOpenSoundDevice() { return TRUE; }

void S9xToggleSoundChannel(int c) {}

void S9xHandlePortCommand(s9xcommand_t cmd, int16 data1, int16 data2) {}

void S9xSyncSpeed() {}

void S9xExtraUsage() {}

void S9xParseArg(char **argv, int &i, int argc) {}

void S9xParsePortConfig(ConfigFile &conf, int pass) {}

bool S9xPollButton(uint32 id, bool *pressed) { return false; }

bool S9xPollAxis(uint32 id, int16 *value) { return false; }

bool S9xPollPointer(uint32 id, int16 *x, int16 *y) { return false; }

const char *S9xChooseFilename(bool8 read_only) { return nullptr; }

const char *S9xChooseMovieFilename(bool8 read_only) { return nullptr; }

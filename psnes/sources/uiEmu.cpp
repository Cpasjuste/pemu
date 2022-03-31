//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "uiEmu.h"
#include "video.h"

#include <snes9x.h>
#include <memmap.h>
#include <apu/apu.h>
#include <controls.h>
#include <filter/blit.h>
#include <conffile.h>
#include <display.h>
#include <sys/stat.h>
#include <cheats.h>

#ifdef __PSP2__

#include <psp2/io/dirent.h>
#include <psp2/kernel/threadmgr.h>

#define mkdir(x, y) sceIoMkdir(x, 0777)
#define usleep sceKernelDelayThread
#endif

using namespace c2d;
using namespace c2dui;

#ifndef NDEBUG
C2DClock timer;
#endif

static UiMain *_ui;

typedef void (*Blitter)(uint8 *, int, uint8 *, int, int, int);

#ifdef __SOFT_SCALERS__
enum {
    VIDEOMODE_BLOCKY = 0,
    VIDEOMODE_TV,
    VIDEOMODE_SMOOTH,
    VIDEOMODE_SUPEREAGLE,
    VIDEOMODE_2XSAI,
    VIDEOMODE_SUPER2XSAI,
    VIDEOMODE_EPX,
    VIDEOMODE_HQ2X
};
#endif

static void *audio_buffer = nullptr;
static uint8 *gfx_snes_buffer = nullptr;
static uint8 *gfx_video_buffer = nullptr;
static int snes9x_prev_width = 0, snes9x_prev_height = 0;

Vector2f snes9x_game_size;

static const char *s9x_base_dir = nullptr;

static char default_dir[PATH_MAX + 1];

static const char dirNames[13][32] =
        {
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

static int make_snes9x_dirs(void);

std::string getButtonId(int player, const std::string &name) {
    return "Joypad" + std::to_string(player) + " " + name;
}

PSNESUiEmu::PSNESUiEmu(UiMain *ui) : UiEmu(ui) {
    printf("PSNESUIEmu()\n");
    _ui = ui;
}

int PSNESUiEmu::load(const ss_api::Game &game) {

    ui->getUiProgressBox()->setTitle(game.name);
    ui->getUiProgressBox()->setMessage("Please wait...");
    ui->getUiProgressBox()->setProgress(0);
    ui->getUiProgressBox()->setVisibility(Visibility::Visible);
    ui->getUiProgressBox()->setLayer(1000);
    ui->flip();

    strncpy(default_dir, ui->getIo()->getDataPath().c_str(), PATH_MAX);
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
    Settings.SoundSync = FALSE;
    Settings.SoundInputRate = 31950;
#ifdef __VITA__
    Settings.SoundPlaybackRate = 22050;
#else
    Settings.SoundPlaybackRate = 48000;
#endif
    // audio
    Settings.Transparency =
            ui->getConfig()->get(Option::ROM_PSNES_TRANSPARENCY, true)->getIndex();
    Settings.AutoDisplayMessages =
            ui->getConfig()->get(Option::ROM_PSNES_DISPLAY_MESSAGES, true)->getIndex();
    Settings.InitialInfoStringTimeout = 120;
    Settings.HDMATimingHack = 100;
    Settings.BlockInvalidVRAMAccessMaster =
            ui->getConfig()->get(Option::ROM_PSNES_BLOCK_VRAM, true)->getIndex();
    Settings.StopEmulation = TRUE;
    Settings.WrongMovieStateProtection = TRUE;
    Settings.DumpStreamsMaxFrames = -1;
    Settings.StretchScreenshots = 1;
    Settings.SnapshotScreenshots = TRUE;
    Settings.FastSavestates = TRUE;

    int skipFramesCfg = ui->getConfig()->get(Option::ROM_PSNES_FRAMESKIP, true)->getIndex();
    if (skipFramesCfg == 0) {
        Settings.SkipFrames = 0;
    } else if (skipFramesCfg == 1) {
        Settings.SkipFrames = AUTO_FRAMERATE;
    } else {
        Settings.SkipFrames = skipFramesCfg - 1;
    }
    printf("Settings.SkipFrames: %i\n", Settings.SkipFrames);
    Settings.TurboMode = ui->getConfig()->get(Option::ROM_PSNES_TURBO_MODE, true)->getIndex();
    Settings.TurboSkipFrames = ui->getConfig()->get(Option::ROM_PSNES_TURBO_FRAMESKIP, true)->getIndex();

    Settings.CartAName[0] = 0;
    Settings.CartBName[0] = 0;

    // big boost when SupportHiRes disabled
    Settings.SupportHiRes = (bool8) ui->getConfig()->get(Option::ROM_PSNES_HIGH_RES, true)->getIndex();
    printf("Settings.SupportHiRes: %i\n", Settings.SupportHiRes);

    CPU.Flags = 0;

    make_snes9x_dirs();

    S9xDeleteCheats();

    if (!Memory.Init() || !S9xInitAPU()) {
        printf("Could not initialize Snes9x Memory.\n");
        ui->getUiProgressBox()->setVisibility(Visibility::Hidden);
        stop();
        return -1;
    }

    S9xInitSound(32);
    S9xSetSoundMute(FALSE);
    S9xSetSamplesAvailableCallback(nullptr, nullptr);

    //getButtonId
    S9xUnmapAllControls();
    for (uint32 i = 0; i < 4; i++) {
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
        ui->getUiProgressBox()->setVisibility(Visibility::Hidden);
        ui->getUiMessageBox()->show("ERROR", "INVALID ROM", "OK");
        stop();
        return -1;
    }

    Memory.LoadSRAM(S9xGetFilename(".srm", SRAM_DIR));

    Settings.ApplyCheats = FALSE;
    if (ui->getConfig()->get(Option::ROM_PSNES_CHEATS, true)->getIndex() == 1) {
        printf("Settings.ApplyCheats = TRUE\n");
        Settings.ApplyCheats = TRUE;
    }
    S9xDeleteCheats();
    S9xCheatsEnable();
    if (Settings.ApplyCheats) {
        printf("S9xLoadCheatFile(%s)\n", S9xGetFilename(".cht", CHEAT_DIR));
        S9xLoadCheatFile(S9xGetFilename(".cht", CHEAT_DIR));
    }

    CPU.Flags = saved_flags;
    Settings.StopEmulation = FALSE;

    // Initialize filters
    S9xBlitFilterInit();
#ifdef __SOFT_SCALERS__
    S9xBlit2xSaIFilterInit();
    S9xBlitHQ2xFilterInit();
#endif

    if (Settings.SupportHiRes) {
        GFX.Pitch = SNES_WIDTH * 2 * 2;
        gfx_snes_buffer = (uint8 *) malloc(GFX.Pitch * ((SNES_HEIGHT_EXTENDED + 4) * 2));
        memset(gfx_snes_buffer, 0, GFX.Pitch * ((SNES_HEIGHT_EXTENDED + 4) * 2));
        GFX.Screen = (uint16 *) gfx_snes_buffer;
        snes9x_game_size = {SNES_WIDTH * 2, SNES_HEIGHT_EXTENDED * 2};
        auto v = new PSNESVideo(getUi(), &gfx_video_buffer, nullptr,
                                Vector2i(SNES_WIDTH * 2, SNES_HEIGHT_EXTENDED * 2));
        addVideo(v);
        memset(gfx_video_buffer, 0, (size_t) getVideo()->getTexture()->m_pitch * getVideo()->getTextureRect().height);
    } else {
        GFX.Pitch = SNES_WIDTH * 2;
        snes9x_game_size = {SNES_WIDTH, SNES_HEIGHT_EXTENDED};
        auto v = new PSNESVideo(getUi(), (uint8_t **) &GFX.Screen, (int *) &GFX.Pitch,
                                Vector2i(SNES_WIDTH, SNES_HEIGHT_EXTENDED));
        addVideo(v);
        memset(GFX.Screen, 0, (size_t) getVideo()->getTexture()->m_pitch * getVideo()->getTextureRect().height);
    }

    S9xGraphicsInit();

    int samples = Audio::toSamples((int) Settings.SoundPlaybackRate, (float) Memory.ROMFramesPerSecond);
    addAudio((int) Settings.SoundPlaybackRate, samples);
    audio_buffer = malloc(getAudio()->getSamplesSize() * 2);

    targetFps = (float) Memory.ROMFramesPerSecond;

    ui->getUiProgressBox()->setProgress(1);
    ui->flip();
    ui->delay(500);
    ui->getUiProgressBox()->setVisibility(Visibility::Hidden);

    return UiEmu::load(game);
}

void PSNESUiEmu::stop() {

    Settings.StopEmulation = TRUE;

    Memory.SaveSRAM(S9xGetFilename(".srm", SRAM_DIR));
    //S9xResetSaveTimer(FALSE);
    //S9xSaveCheatFile(S9xGetFilename(".bml", CHEAT_DIR));

    S9xBlitFilterDeinit();
#ifdef __SOFT_SCALERS__
    S9xBlit2xSaIFilterDeinit();
    S9xBlitHQ2xFilterDeinit();
#endif

    S9xUnmapAllControls();
    S9xGraphicsDeinit();
    Memory.Deinit();
    S9xDeinitAPU();

    if (gfx_snes_buffer != nullptr) {
        free(gfx_snes_buffer);
        gfx_snes_buffer = nullptr;
    }

    snes9x_prev_width = 0;
    snes9x_prev_height = 0;

    if (audio_buffer != nullptr) {
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
        auto players = ui->getInput()->getPlayers();

        // update snes9x buttons
        for (uint32 i = 0; i < 4; i++) {
            S9xReportButton(0 + (i * 12), (players[i].keys & Input::Key::Up) > 0);
            S9xReportButton(1 + (i * 12), (players[i].keys & Input::Key::Down) > 0);
            S9xReportButton(2 + (i * 12), (players[i].keys & Input::Key::Left) > 0);
            S9xReportButton(3 + (i * 12), (players[i].keys & Input::Key::Right) > 0);
            S9xReportButton(4 + (i * 12), (players[i].keys & Input::Key::Fire1) > 0);
            S9xReportButton(5 + (i * 12), (players[i].keys & Input::Key::Fire2) > 0);
            S9xReportButton(6 + (i * 12), (players[i].keys & Input::Key::Fire3) > 0);
            S9xReportButton(7 + (i * 12), (players[i].keys & Input::Key::Fire4) > 0);
            S9xReportButton(8 + (i * 12), (players[i].keys & Input::Key::Fire5) > 0);
            S9xReportButton(9 + (i * 12), (players[i].keys & Input::Key::Fire6) > 0);
            S9xReportButton(10 + (i * 12), (players[i].keys & Input::Key::Start) > 0);
            S9xReportButton(11 + (i * 12), (players[i].keys & Input::Key::Select) > 0);
        }

        S9xMainLoop();

        int samples = S9xGetSampleCount();
        S9xMixSamples((uint8 *) audio_buffer, samples);
        _ui->getUiEmu()->getAudio()->play(audio_buffer, samples >> 1, true);
    }
}

void PSNESUiEmu::pause() {
    S9xSetSoundMute(TRUE);
    UiEmu::pause();
}

void PSNESUiEmu::resume() {
    S9xSetSoundMute(FALSE);
    UiEmu::resume();
}

///////////////////////////////////////////////////////////////////////////////
// Functions called by Snes9x below
///////////////////////////////////////////////////////////////////////////////

/**
 * Called just before Snes9x begins to render an SNES screen.
 * Use this function if you should prepare before drawing, otherwise let it empty.
 */
bool8 S9xInitUpdate() {
    return TRUE;
}

/**
 * Called once a complete SNES screen has been rendered into the GFX.Screen memory buffer,
 * now is your chance to copy the SNES rendered screen to the host computer's screen memory.
 * The problem is that you have to cope with different sized SNES rendered screens:
 * 256*224, 256*239, 512*224, 512*239, 512*448 and 512*478.
 */
bool8 S9xDeinitUpdate(int width, int height) {

    Blitter blit = nullptr;
#ifdef __SOFT_SCALERS__
    int effect = _ui->getConfig()->getValue(Option::ROM_SHADER, true);
#else
    int effect = 0;
#endif
    // for video.cpp scaling
    //printf("S9xDeinitUpdate: %i x %i\n", width, height);
    C2DUIVideo *video = _ui->getUiEmu()->getVideo();

    if (snes9x_prev_height != height) {
        if (Settings.SupportHiRes) {
            snes9x_game_size = {(float) width * 2, (float) height * 2};
        } else {
            snes9x_game_size = {(float) width, (float) height};
        }
        printf("video->updateScaling: res: %i x %i\n", width, height);
        video->updateScaling();
    }

    if (Settings.SupportHiRes) {
#ifdef __SOFT_SCALERS__
        if (effect == VIDEOMODE_BLOCKY || effect == VIDEOMODE_TV || effect == VIDEOMODE_SMOOTH) {
#endif
        if ((width <= SNES_WIDTH) && ((snes9x_prev_width != width) || (snes9x_prev_height != height))) {
            printf("S9xBlitClearDelta\n");
            S9xBlitClearDelta();
        }
#ifdef __SOFT_SCALERS__
        }
#endif

        if (width <= SNES_WIDTH) {
            if (height > SNES_HEIGHT_EXTENDED) {
                blit = S9xBlitPixSimple2x1;
            } else {
                switch (effect) {
#ifdef __SOFT_SCALERS__
                    case VIDEOMODE_TV:
                        blit = S9xBlitPixTV2x2;
                        break;
                    case VIDEOMODE_SMOOTH:
                        blit = S9xBlitPixSmooth2x2;
                        break;
                    case VIDEOMODE_SUPEREAGLE:
                        blit = S9xBlitPixSuperEagle16;
                        break;
                    case VIDEOMODE_2XSAI:
                        blit = S9xBlitPix2xSaI16;
                        break;
                    case VIDEOMODE_SUPER2XSAI:
                        blit = S9xBlitPixSuper2xSaI16;
                        break;
                    case VIDEOMODE_EPX:
                        blit = S9xBlitPixEPX16;
                        break;
                    case VIDEOMODE_HQ2X:
                        blit = S9xBlitPixHQ2x16;
                        break;
#endif
                    default:
                        blit = S9xBlitPixSimple2x2;
                        break;
                }
            }
        } else if (height <= SNES_HEIGHT_EXTENDED) {
            switch (effect) {
                default:
                    blit = S9xBlitPixSimple1x2;
                    break;
#ifdef __SOFT_SCALERS__
                    case VIDEOMODE_TV:
                        blit = S9xBlitPixTV1x2;
                        break;
#endif
            }
        } else {
            blit = S9xBlitPixSimple1x1;
        }

        blit((uint8 *) GFX.Screen, GFX.Pitch, gfx_video_buffer, video->getTexture()->m_pitch, width, height);

        if (height < snes9x_prev_height) {
            int p = video->getTexture()->m_pitch >> 2;
            for (int y = SNES_HEIGHT * 2; y < SNES_HEIGHT_EXTENDED * 2; y++) {
                auto *d = (uint32 *) (gfx_video_buffer + y * video->getTexture()->m_pitch);
                for (int x = 0; x < p; x++)
                    *d++ = 0;
            }
        }
    } else {
        if (height < snes9x_prev_height) {
            int p = video->getTexture()->m_pitch >> 2;
            for (int y = SNES_HEIGHT; y < SNES_HEIGHT_EXTENDED; y++) {
                auto *d = (uint32 *) (GFX.Screen + y * video->getTexture()->m_pitch);
                for (int x = 0; x < p; x++)
                    *d++ = 0;
            }
        }
    }

    snes9x_prev_width = width;
    snes9x_prev_height = height;

    video->getTexture()->unlock();

    return TRUE;
}

/**
 * Called at the end of screen refresh if GFX.DoInterlace && GFX.InterlaceFrame == 0 is true (?).
 */
bool8 S9xContinueUpdate(int width, int height) {
    return TRUE;
}

/**
 * Display port-specific usage information
 */
void S9xExtraUsage() {
}

/**
 * Parse port-specific arguments
 */
void S9xParseArg(char **argv, int &i, int argc) {
}

/**
 * Parse port-specific config
 */
void S9xParsePortConfig(ConfigFile &conf, int pass) {
}

static int make_snes9x_dirs() {

    if (strlen(s9x_base_dir) + 1 + sizeof(dirNames[0]) > PATH_MAX + 1)
        return (-1);

    mkdir(s9x_base_dir, 0755);

    for (int i = 0; i < LAST_DIR; i++) {
        if (dirNames[i][0]) {
            char s[PATH_MAX + 1];
            snprintf(s, PATH_MAX + 1, "%s%s%s", s9x_base_dir, SLASH_STR, dirNames[i]);
            mkdir(s, 0755);
        }
    }

    return (0);
}

/**
 * Called when Snes9x wants to know the directory dirtype.
 */
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

/**
 * When Snes9x needs to know the name of the cheat/IPS file and so on, this function is called.
 * Check extension and dirtype, and return the appropriate filename.
 * The current ports return the ROM file path with the given extension.
 */
const char *S9xGetFilename(const char *ex, s9x_getdirtype dirtype) {

    static char s[PATH_MAX + 1];
    char drive[_MAX_DRIVE + 1], dir[_MAX_DIR + 1], fname[_MAX_FNAME + 1], ext[_MAX_EXT + 1];

    _splitpath(Memory.ROMFilename, drive, dir, fname, ext);
    snprintf(s, PATH_MAX + 1, "%s%s%s%s", S9xGetDirectory(dirtype), SLASH_STR, fname, ex);

    return (s);
}

/**
 * Almost the same as S9xGetFilename function, but used for saving SPC files etc.
 * So you have to take care not to delete the previously saved file, by increasing
 * the number of the filename; romname.000.spc, romname.001.spc, ...
 */
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

/**
 * Called when Snes9x wants to know the name of the ROM image.
 * Typically, extract the filename from path and return it.
 */
const char *S9xBasename(const char *f) {

    const char *p;

    if ((p = strrchr(f, '/')) != NULL || (p = strrchr(f, '\\')) != NULL)
        return (p + 1);

    return (f);
}

/**
 * If your port can match Snes9x's built-in LoadFreezeFile/SaveFreezeFile command
 * (see controls.cpp), you may choose to use this function. Otherwise return NULL.
 */
const char *S9xChooseFilename(bool8 read_only) {
    return nullptr;
}

/**
 * If your port can match Snes9x's built-in BeginRecordingMovie/LoadMovie command
 * (see controls.cpp), you may choose to use this function. Otherwise return NULL.
 */
const char *S9xChooseMovieFilename(bool8 read_only) {
    return nullptr;
}

/**
 * This function opens a freeze-game file. STREAM is defined as a gzFile if ZLIB is
 * defined else it's defined as FILE *. The read_only parameter is set to true when
 * reading a freeze-game file and false when writing a freeze-game file.
 * Open the file filepath and return its pointer file.
 */
bool8 S9xOpenSnapshotFile(const char *filename, bool8 read_only, STREAM*file) {

    if ((*file = OPEN_STREAM(filename, read_only ? "rb" : "wb")))
        return (TRUE);

    printf("S9xOpenSnapshotFile: %s (open failed, read only=%i)\n", filename, read_only);

    return (FALSE);
}

/**
 * This function closes the freeze-game file opened by S9xOpenSnapshotFile function.
 */
void S9xCloseSnapshotFile(STREAM file) {

    printf("S9xCloseSnapshotFile\n");
    CLOSE_STREAM(file);
}

/**
 * If Settings.AutoSaveDelay is not zero, Snes9x calls this function when the contents of
 * the S-RAM has been changed. Typically, call Memory.SaveSRAM function from this function.
 */
void S9xAutoSaveSRAM() {

    Memory.SaveSRAM(S9xGetFilename(".srm", SRAM_DIR));
}

/**
 * Called at the end of S9xMainLoop function, when emulating one frame has been done.
 * You should adjust the frame rate in this function
 */
void S9xSyncSpeed() {
}

/**
 * Called by Snes9x to poll for buttons that should be polled.
 */
bool S9xPollButton(uint32 id, bool *pressed) {
    return false;
}

/**
 * Called by Snes9x to poll for axises that should be polled.
 */
bool S9xPollAxis(uint32 id, int16 *value) {
    return false;
}

/**
 * Called by Snes9x to poll for poiters that should be polled.
 */
bool S9xPollPointer(uint32 id, int16 *x, int16 *y) {
    return false;
}

/**
 * Handle port-specific commands (?).
 */
void S9xHandlePortCommand(s9xcommand_t cmd, int16 data1, int16 data2) {
}

/**
 * Called when some fatal error situation arises or when the “q” debugger command is used.
 */
void S9xExit() {
    _ui->getUiEmu()->stop();
}

/**
 * When Snes9x wants to display an error, information or warning message, it calls this function.
 * Check in messages.h for the types and individual message numbers that Snes9x currently passes as parameters.
 * The idea is display the message string so the user can see it, but you choose not to display anything at all,
 * or change the message based on the message number or message type.
 * Eventually all debug output will also go via this function, trace information already does.
 */
void S9xMessage(int type, int number, const char *message) {
    const int max = 36 * 3;
    static char buffer[max + 1];
    printf("[%i][%i]: %s\n", type, number, message);
    strncpy(buffer, message, max + 1);
    buffer[max] = 0;
    S9xSetInfoString(buffer);
}

/**
 * Used by Snes9x to ask the user for input.
 */
const char *S9xStringInput(const char *message) {
    return nullptr;
}

/**
 * Called when the SNES color palette has changed.
 * Use this function if your system should change its color palette to match the SNES's.
 * Otherwise let it empty.
 */
void S9xSetPalette() {}

bool8 S9xOpenSoundDevice() {
    return TRUE;
}

void S9xToggleSoundChannel(int c) {}

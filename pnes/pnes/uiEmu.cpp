//
// Created by cpasjuste on 01/06/18.
//

#include <fstream>

#include "c2dui.h"
#include "uiEmu.h"

#include "common/nstcommon.h"
#include "common/video.h"
#include "common/config.h"
#include "common/input.h"
#include "audio.h"

using namespace c2dui;

extern PNESGuiEmu *uiEmu;

/// NESTOPIA
settings_t conf;

extern nstpaths_t nstpaths;

extern bool (*nst_archive_select)(const char *, char *, size_t);

extern Input::Controllers *cNstPads;

extern Emulator emulator;
/// NESTOPIA

PNESGuiEmu::PNESGuiEmu(UIMain *ui) : UIEmu(ui) {
    printf("PNESGuiEmu()\n");
}

int PNESGuiEmu::load(RomList::Rom *rom) {

    getUi()->getUiProgressBox()->setTitle(rom->name);
    getUi()->getUiProgressBox()->setMessage("Please wait...");
    getUi()->getUiProgressBox()->setProgress(0);
    getUi()->getUiProgressBox()->setVisibility(c2d::Visibility::Visible);
    getUi()->getUiProgressBox()->setLayer(1000);
    getUi()->flip();

    nestopia_config_init();

    std::string file = std::string(*getUi()->getConfig()->getRomPath(0) + rom->name);
    if (nestopia_core_init(file.c_str()) != 0) {
        getUi()->getUiProgressBox()->setVisibility(c2d::Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "INVALID FILE", "OK");
        stop();
        return -1;
    }

    getUi()->getUiProgressBox()->setProgress(1);
    getUi()->flip();
    getUi()->delay(500);
    getUi()->getUiProgressBox()->setVisibility(c2d::Visibility::Hidden);

    return UIEmu::load(rom);
}

void PNESGuiEmu::stop() {

    // Remove the cartridge and shut down the NES
    nst_unload();

    // Unload the FDS BIOS, NstDatabase.xml, and the custom palette
    nst_db_unload();
    nst_fds_bios_unload();
    nst_palette_unload();

    UIEmu::stop();
}

bool PNESGuiEmu::onInput(c2d::Input::Player *players) {

    if (getUi()->getUiMenu()->isVisible()
        || getUi()->getUiStateMenu()->isVisible()) {
        return UIEmu::onInput(players);
    }

    // look for player 1 menu combo
    if (((players[0].keys & c2d::Input::Key::Start) && (players[0].keys & c2d::Input::Key::Select))) {
        pause();
        getUi()->getConfig()->load(getUi()->getUiRomList()->getSelection());
        getUi()->getUiMenu()->load(true);
        return true;
    } else if (((players[0].keys & c2d::Input::Key::Start) && (players[0].keys & c2d::Input::Key::Fire5))
               || ((players[0].keys & c2d::Input::Key::Select) && (players[0].keys & c2d::Input::Key::Fire5))
               || ((players[0].keys & c2d::Input::Key::Start) && (players[0].keys & c2d::Input::Key::Fire6))
               || ((players[0].keys & c2d::Input::Key::Select) && (players[0].keys & c2d::Input::Key::Fire6))) {
        pause();
        getUi()->getConfig()->load(getUi()->getUiRomList()->getSelection());
        getUi()->getUiMenu()->load(true);
        return true;
    }

    // TODO: this cause some problem where we can't send start or select on handled mode
    /*
    // look each players for combos keys
    for (int i = 0; i < NUMGAMEPADS; i++) {
        // allow devices with single select/start button to send start/coins (nsw in single joycon mode)
        if (((players[i].state & c2d::Input::Key::KEY_START) && (players[i].state & c2d::Input::Key::KEY_FIRE1))
            || ((players[i].state & c2d::Input::Key::KEY_COIN) && (players[i].state & c2d::Input::Key::KEY_FIRE1))) {
            players[i].state = c2d::Input::Key::KEY_START;
        } else if (((players[i].state & c2d::Input::Key::KEY_START) && (players[i].state & c2d::Input::Key::KEY_FIRE2))
                   || ((players[i].state & c2d::Input::Key::KEY_COIN) &&
                       (players[i].state & c2d::Input::Key::KEY_FIRE2))) {
            players[i].state = c2d::Input::Key::KEY_COIN;
        }
    }
    */

    // look for window resize event
    if (players[0].keys & EV_RESIZE) {
        // useful for sdl resize event
        getVideo()->updateScaling();
    }

    return true;
}

void PNESGuiEmu::onDraw(c2d::Transform &transform, bool draw) {

    if (!isPaused()) {
        // fps
        int showFps = getUi()->getConfig()->get(Option::Index::ROM_SHOW_FPS, true)->getValueBool();
        getFpsText()->setVisibility(showFps ? c2d::Visibility::Visible : c2d::Visibility::Hidden);
        if (showFps) {
            sprintf(getFpsString(), "FPS: %.2g/%2d", getUi()->getFps(),
                    nst_pal() ? (conf.timing_speed / 6) * 5 : conf.timing_speed);
            getFpsText()->setString(getFpsString());
        }

        // update nestopia buttons
        auto players = getUi()->getInput()->getPlayers();

        if (players[0].keys & c2d::Input::Key::Fire3) {
            nst_set_rewind(0);
        } else if (Rewinder(emulator).GetDirection() == Rewinder::BACKWARD) {
            nst_set_rewind(1);
        }

        for (int i = 0; i < NUMGAMEPADS; i++) {
            cNstPads->pad[i].buttons = 0;

            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Start) > 0 ?
                                        Input::Controllers::Pad::START : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Select) > 0 ?
                                        Input::Controllers::Pad::SELECT : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Up) > 0 ?
                                        Input::Controllers::Pad::UP : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Down) > 0 ?
                                        Input::Controllers::Pad::DOWN : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Left) > 0 ?
                                        Input::Controllers::Pad::LEFT : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Right) > 0 ?
                                        Input::Controllers::Pad::RIGHT : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire1) > 0 ?
                                        Input::Controllers::Pad::B : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire2) > 0 ?
                                        Input::Controllers::Pad::A : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire4) > 0 ?
                                        Input::Controllers::Pad::B : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire5) > 0 ?
                                        Input::Controllers::Pad::A : 0;
        }

        // step nestopia core
        nst_emuloop();
    }

    UIEmu::onDraw(transform, draw);
}

/// NESTOPIA

/// NESTOPIA AUDIO
void (*audio_deinit)();

void audio_deinit_dummy() {}

void audio_init() {
    uiEmu->addAudio(conf.audio_sample_rate, nst_pal() ? (conf.timing_speed / 6) * 5 : conf.timing_speed);
}

void audio_play() {
    if (uiEmu->getAudio()) {
        uiEmu->getAudio()->play(true);
    }
}

void audio_pause() {
    if (uiEmu->getAudio()) {
        uiEmu->getAudio()->pause(1);
    }
}

void audio_unpause() {
    if (uiEmu->getAudio()) {
        uiEmu->getAudio()->pause(0);
    }
}

void audio_set_params(Sound::Output *soundoutput) {

    c2d::Audio *aud = uiEmu->getAudio();

    if (aud) {
        // Set audio parameters
        Sound sound(emulator);
        sound.SetSampleBits(16);
        sound.SetSampleRate((ulong) conf.audio_sample_rate);
        sound.SetSpeaker(Sound::SPEAKER_STEREO);
        sound.SetSpeed(Sound::DEFAULT_SPEED);
        //audio_adj_volume();
        soundoutput->samples[0] = aud->getBuffer();
        soundoutput->length[0] = (unsigned int) aud->getBufferLen();
        soundoutput->samples[1] = nullptr;
        soundoutput->length[1] = 0;
    }
}

/// NESTOPIA CONFIG
void PNESGuiEmu::nestopia_config_init() {

    // Video
    conf.video_filter = 0;
    conf.video_scale_factor = 1;
    conf.video_palette_mode = 0;
    conf.video_decoder = 0;
    conf.video_brightness = 0; // -100 to 100
    conf.video_saturation = 0; // -100 to 100
    conf.video_contrast = 0; // -100 to 100
    conf.video_hue = 0; // -45 to 45
    conf.video_ntsc_mode = 0;
    conf.video_ntsc_sharpness = 0; // -100 to 100
    conf.video_ntsc_resolution = 0; // -100 to 100
    conf.video_ntsc_bleed = 0; // -100 to 100
    conf.video_ntsc_artifacts = 0; // -100 to 100
    conf.video_ntsc_fringing = 0; // -100 to 100
    conf.video_xbr_corner_rounding = 0;
    conf.video_linear_filter = false;
    conf.video_tv_aspect = false;
    conf.video_unmask_overscan = false;
    conf.video_fullscreen = false;
    conf.video_stretch_aspect = false;
    conf.video_unlimited_sprites = false;
    conf.video_xbr_pixel_blending = false;

    // Audio
    conf.audio_api = 0;
    conf.audio_stereo = false;
    conf.audio_sample_rate = 48000;
    conf.audio_volume = 85;
    conf.audio_vol_sq1 = 85;
    conf.audio_vol_sq2 = 85;
    conf.audio_vol_tri = 85;
    conf.audio_vol_noise = 85;
    conf.audio_vol_dpcm = 85;
    conf.audio_vol_fds = 85;
    conf.audio_vol_mmc5 = 85;
    conf.audio_vol_vrc6 = 85;
    conf.audio_vol_vrc7 = 85;
    conf.audio_vol_n163 = 85;
    conf.audio_vol_s5b = 85;

    // Timing
    conf.timing_speed = 60;
    conf.timing_ffspeed = 3;
    conf.timing_turbopulse = 3;
    conf.timing_vsync = true;
    conf.timing_limiter = true;

    // Misc
    conf.misc_default_system = 0;
    conf.misc_soft_patching = true;
    //conf.misc_suppress_screensaver = true;
    conf.misc_genie_distortion = false;
    //conf.misc_disable_gui = false;
    conf.misc_disable_cursor = false;
    conf.misc_disable_cursor_special = false;
    conf.misc_config_pause = false;
    conf.misc_last_folder = nullptr;
    conf.misc_power_state = 0;
    conf.misc_overclock = false;
    conf.misc_homebrew_exit = -1;
    conf.misc_homebrew_stdout = -1;
    conf.misc_homebrew_stderr = -1;
}

// NESTOPIA CORE INIT
int PNESGuiEmu::nestopia_core_init(const char *rom_path) {

    // Set up directories
    nst_set_dirs();

    // Set up callbacks
    nst_set_callbacks();

    nst_input_turbo_init();

    // Set archive handler function pointer
    nst_archive_select = &nst_archive_select_file;

    // Set audio function pointers
    audio_deinit = &audio_deinit_dummy;

    // Set the video dimensions
    video_set_dimensions();

    // Initialize and load FDS BIOS and NstDatabase.xml
    nst_fds_bios_load();
    nst_db_load();

    if (!nst_load(rom_path)) {
        return -1;
    }

    nst_video_set_dimensions_screen(nst_video_get_dimensions_screen());

    // Set play in motion
    nst_play();

    return 0;
}

int nestopia_state_load(const char *path) {

    int ret = -1;
    std::filebuf fb;

    if (fb.open(path, std::ios::in)) {
        std::istream is(&fb);
        Machine machine(emulator);
        ret = machine.LoadState(is);
        fb.close();
    }

    return ret;
}

int nestopia_state_save(const char *path) {

    int ret;
    std::ofstream ofs;

    ofs.open(path, std::ofstream::out | std::ofstream::binary);
    Machine machine(emulator);
    ret = machine.SaveState(ofs, Nes::Api::Machine::NO_COMPRESSION);
    ofs.close();

    return ret;
}

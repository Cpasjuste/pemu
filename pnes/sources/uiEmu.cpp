//
// Created by cpasjuste on 01/06/18.
//

#include <fstream>

#include "common/nstcommon.h"
#include "common/video.h"
#include "common/config.h"
#include "common/input.h"
#include "audio.h"

#include "c2dui.h"
#include "uiEmu.h"

extern PNESGuiEmu *uiEmu;

/// NESTOPIA
settings_t conf;

extern nstpaths_t nstpaths;

extern bool (*nst_archive_select)(const char *, char *, size_t);

extern Nes::Core::Input::Controllers *cNstPads;

extern Emulator emulator;
/// NESTOPIA

PNESGuiEmu::PNESGuiEmu(UiMain *ui) : UIEmu(ui) {
    printf("PNESGuiEmu()\n");
}

int PNESGuiEmu::load(const ss_api::Game &game) {

    getUi()->getUiProgressBox()->setTitle(game.getName().text);
    getUi()->getUiProgressBox()->setMessage("Please wait...");
    getUi()->getUiProgressBox()->setProgress(0);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Visible);
    getUi()->getUiProgressBox()->setLayer(1000);
    getUi()->flip();

    nestopia_config_init();

    std::string fullPath = game.romsPath + game.path;
    if (nestopia_core_init(fullPath.c_str()) != 0) {
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "INVALID FILE", "OK");
        stop();
        return -1;
    }

    getUi()->getUiProgressBox()->setProgress(1);
    getUi()->flip();
    getUi()->delay(500);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);

    return UIEmu::load(game);
}

void PNESGuiEmu::stop() {

    nst_pause();

    // Remove the cartridge and shut down the NES
    nst_unload();

    // Unload the FDS BIOS, NstDatabase.xml, and the custom palette
    nst_db_unload();
    nst_fds_bios_unload();
    nst_palette_unload();

    UIEmu::stop();
}

bool PNESGuiEmu::onInput(c2d::Input::Player *players) {
    return UIEmu::onInput(players);
}

void PNESGuiEmu::onUpdate() {

    if (!isPaused()) {

        // fps
        int showFps = getUi()->getConfig()->get(Option::Id::ROM_SHOW_FPS, true)->getValueBool();
        getFpsText()->setVisibility(showFps ? Visibility::Visible : Visibility::Hidden);
        if (showFps) {
            sprintf(getFpsString(), "FPS: %.2g/%2d", getUi()->getFps(), nst_pal() ? 50 : 60);
            getFpsText()->setString(getFpsString());
        }

        // update nestopia buttons
        auto *players = getUi()->getInput()->getPlayers();

        if (players[0].keys & c2d::Input::Key::Fire3) {
            nst_set_rewind(0);
        } else if (Rewinder(emulator).GetDirection() == Rewinder::BACKWARD) {
            nst_set_rewind(1);
        }

        for (int i = 0; i < NUMGAMEPADS; i++) {
            cNstPads->pad[i].buttons = 0;

            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Start) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::START : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Select) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::SELECT : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Up) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::UP : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Down) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::DOWN : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Left) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::LEFT : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Right) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::RIGHT : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire1) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::B : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire2) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::A : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire4) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::B : 0;
            cNstPads->pad[i].buttons |= (players[i].keys & c2d::Input::Key::Fire5) > 0 ?
                                        Nes::Core::Input::Controllers::Pad::A : 0;
        }

        // step nestopia core
        nst_emuloop();
    }

    UIEmu::onUpdate();
}

/// NESTOPIA

/// NESTOPIA AUDIO
static void *audio_buffer = nullptr;

void audio_init() {
    int samples = conf.audio_sample_rate / (nst_pal() ? 50 : 60);
    uiEmu->addAudio(conf.audio_sample_rate, samples);
}

void audio_deinit() {
    if (audio_buffer) {
        free(audio_buffer);
        audio_buffer = nullptr;
    }
}

void audio_queue() {
    if (uiEmu->getAudio() != nullptr) {
        uiEmu->getAudio()->play(audio_buffer, uiEmu->getAudio()->getSamples(), true);
    }
}

void audio_pause() {
    if (uiEmu->getAudio() != nullptr) {
        uiEmu->getAudio()->pause(1);
    }
}

void audio_unpause() {
    if (uiEmu->getAudio() != nullptr) {
        uiEmu->getAudio()->pause(0);
    }
}

void audio_set_speed(int speed) {
    // TODO
    //bufsize = (channels * (conf.audio_sample_rate / framerate)) / speed;
}

void audio_set_params(Sound::Output *soundoutput) {

    Audio *aud = uiEmu->getAudio();

    if (aud != nullptr) {
        // Set audio parameters
        Sound sound(emulator);
        sound.SetSampleBits(16);
        sound.SetSampleRate((unsigned long) conf.audio_sample_rate);
        sound.SetSpeaker(Sound::SPEAKER_STEREO);
        sound.SetSpeed(Sound::DEFAULT_SPEED);
        audio_buffer = malloc(aud->getSamplesSize());
        soundoutput->samples[0] = audio_buffer;
        soundoutput->length[0] = (unsigned int) aud->getSamples();
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
    conf.audio_stereo = true;
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

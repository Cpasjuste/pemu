//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "pgen_ui_emu.h"
#include "osd.h"

extern "C" {
#include "shared.h"
}

static short sound_buffer[2048];

PGENUiEmu::PGENUiEmu(UiMain *ui) : UiEmu(ui) {
    printf("PGENUiEmu()\n");
    set_config_defaults();
    set_paths_default((ui->getIo()->getDataPath() + "bios").c_str());
}

int PGENUiEmu::load(const ss_api::Game &game) {
    getUi()->getUiProgressBox()->setTitle(game.name);
    getUi()->getUiProgressBox()->setMessage("Please wait...");
    getUi()->getUiProgressBox()->setProgress(0);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Visible);
    getUi()->getUiProgressBox()->setLayer(1000);
    getUi()->flip();

    loadBios();

    // video init
    memset(&bitmap, 0, sizeof(bitmap));
    addVideo(&bitmap.data, &bitmap.pitch, {512, 512});

    // load rom
    std::string path = game.romsPath + game.path;
    if (!load_rom((char *) path.c_str())) {
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "INVALID FILE", "OK");
        stop();
        return -1;
    }

    // system init
    audio_init(48000, 0);
    system_init();
    targetFps = vdp_pal ? 50 : 60;

    // audio init
    int samples = 48000 / (vdp_pal ? 50 : 60);
    addAudio(48000, samples);

    getUi()->getUiProgressBox()->setProgress(1);
    getUi()->flip();
    getUi()->delay(500);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);

    /* reset system hardware */
    system_reset();

    return UiEmu::load(game);
}

void PGENUiEmu::stop() {
    audio_shutdown();
    UiEmu::stop();
}

bool PGENUiEmu::onInput(c2d::Input::Player *players) {
    return UiEmu::onInput(players);
}

void PGENUiEmu::onUpdate() {
    if (!isPaused()) {
        // inputs
        for (int i = 0; i < PLAYER_MAX; i++) {
            unsigned int buttons = getUi()->getInput()->getButtons(i);
            input.pad[i] = 0;
            switch (input.dev[i]) {
                case DEVICE_PAD6B:
                case DEVICE_PAD3B:
                case DEVICE_PAD2B:
                    if (buttons & Input::Button::Up) input.pad[i] |= INPUT_UP;
                    if (buttons & Input::Button::Down) input.pad[i] |= INPUT_DOWN;
                    if (buttons & Input::Button::Left) input.pad[i] |= INPUT_LEFT;
                    if (buttons & Input::Button::Right) input.pad[i] |= INPUT_RIGHT;
                    if (buttons & Input::Button::A) input.pad[i] |= INPUT_A;
                    if (buttons & Input::Button::B) input.pad[i] |= INPUT_B;
                    if (buttons & Input::Button::X) input.pad[i] |= INPUT_C;
                    if (buttons & Input::Button::Y) input.pad[i] |= INPUT_X;
                    if (buttons & Input::Button::LT) input.pad[i] |= INPUT_Y;
                    if (buttons & Input::Button::RT) input.pad[i] |= INPUT_Z;
                    if (buttons & Input::Button::Select) input.pad[i] |= INPUT_MODE;
                    if (buttons & Input::Button::Start) input.pad[i] |= INPUT_START;
                    break;
                default:
                    break;
            }

        }

        if (bitmap.viewport.changed & 4) {
            printf("interlaced mode change: {%i, %i, %i, %i}\n",
                   bitmap.viewport.x, bitmap.viewport.y, bitmap.viewport.w, bitmap.viewport.h);
            resizeVideo();
            bitmap.viewport.changed &= ~4;
        } else if (bitmap.viewport.changed & 1) {
            printf("video mode change: {%i, %i, %i, %i}\n",
                   bitmap.viewport.x, bitmap.viewport.y, bitmap.viewport.w, bitmap.viewport.h);
            resizeVideo();
            bitmap.viewport.changed &= ~1;
        } else if (video->getTextureRect().width != bitmap.viewport.w ||
                   video->getTextureRect().height != bitmap.viewport.h) {
            printf("video rect {%i, %i} != bitmap size: {%i, %i}\n",
                   video->getTextureRect().width, video->getTextureRect().height, bitmap.viewport.w, bitmap.viewport.h);
            resizeVideo();
        }

        // genesis frame step
        if (system_hw == SYSTEM_MCD) {
            system_frame_scd(0);
        } else if ((system_hw & SYSTEM_PBC) == SYSTEM_MD) {
            system_frame_gen(0);
        } else {
            system_frame_sms(0);
        }

        // video
        video->unlock();

        // audio
        int samples = audio_update(sound_buffer);
        getAudio()->play(sound_buffer, samples, vdp_pal);
    }

    return UiEmu::onUpdate();
}

void PGENUiEmu::resizeVideo() {
    video->setTextureRect({0, 0, bitmap.viewport.w, bitmap.viewport.h});
    video->setSize((float) bitmap.viewport.w, (float) bitmap.viewport.h);
    video->lock(&bitmap.data, &bitmap.pitch, {0, 0, bitmap.viewport.w, bitmap.viewport.h});
    video->updateScaling();
}

void PGENUiEmu::loadBios() {
    system_bios = 0;

    /* Genesis BOOT ROM support (2KB max) */
    char *boot_rom_data;
    memset(boot_rom, 0xFF, 0x800);
    size_t size = getUi()->getIo()->read(MD_BIOS, &boot_rom_data, 0x800);
    if (boot_rom_data && size == 0x800) {
        int i;

        memcpy(boot_rom, boot_rom_data, 0x800);
        free(boot_rom_data);

        /* check BOOT ROM */
        if (!memcmp((char *) (boot_rom + 0x120), "GENESIS OS", 10)) {
            /* mark Genesis BIOS as loaded */
            printf("GENESIS BOOT ROM LOADED\n");
            system_bios = SYSTEM_MD;
        }

        /* Byteswap ROM */
        for (i = 0; i < 0x800; i += 2) {
            uint8 temp = boot_rom[i];
            boot_rom[i] = boot_rom[i + 1];
            boot_rom[i + 1] = temp;
        }
    }
}


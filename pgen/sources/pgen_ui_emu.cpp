//
// Created by cpasjuste on 01/06/18.
//

#include <zlib.h>
#include "c2dui.h"
#include "pgen_ui_emu.h"
#include "osd.h"

extern "C" {
#include "shared.h"
}

static short sound_buffer[2048];

static uint32_t brm_crc[2];
static uint8 brm_format[0x40] = {
        0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x40,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x53, 0x45, 0x47, 0x41, 0x5f, 0x43, 0x44, 0x5f, 0x52, 0x4f, 0x4d, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x52, 0x41, 0x4d, 0x5f, 0x43, 0x41, 0x52, 0x54, 0x52, 0x49, 0x44, 0x47, 0x45, 0x5f, 0x5f, 0x5f
};

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

    // check for mega-cd bios before loading a rom
    if (game.system.id == 20) {
        if (!ui->getIo()->exist(CD_BIOS_EU)
            || !ui->getIo()->exist(CD_BIOS_US)
            || !ui->getIo()->exist(CD_BIOS_JP)) {
            getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
            getUi()->getUiMessageBox()->show(
                    "WARNING",
                    "MISSING MEGA-CD BIOS DETECTED, EMULATION WILL LIKELY NOT WORK...",
                    "OK");
        }
    }

    // load genesis bios
    loadBios();

    // video init
    memset(&bitmap, 0, sizeof(bitmap));
    addVideo(&bitmap.data, &bitmap.pitch, {512, 512});

    // load rom
    std::string path = game.romsPath + game.path;
    if (!load_rom((char *) path.c_str())) {
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show(
                "ERROR", "INVALID FILE OR MISSING BIOS...", "OK");
        stop();
        return -1;
    }

    // system init
    audio_init(48000, 0);
    system_init();

    // game gear "special" resolution handling
    if (system_hw == SYSTEM_GG || system_hw == SYSTEM_GGMS) {
        resizeVideo(true);
    }

    // load mega-cd ram if needed
    loadBram();

    // audio init
    targetFps = vdp_pal ? 50 : 60;
    addAudio(48000, vdp_pal ? 966 : 801);

    getUi()->getUiProgressBox()->setProgress(1);
    getUi()->flip();
    getUi()->delay(500);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);

    /* reset system hardware */
    system_reset();

    return UiEmu::load(game);
}

void PGENUiEmu::stop() {
    saveBram();
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

        if (system_hw != SYSTEM_GG && system_hw != SYSTEM_GGMS) {
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
                printf("video rect {%i, %i} != viewport size: {%i, %i}\n",
                       video->getTextureRect().width, video->getTextureRect().height, bitmap.viewport.w,
                       bitmap.viewport.h);
                resizeVideo();
            }
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
#if 0
        int queued = audio->getSampleBufferQueued();
        int capacity = audio->getSampleBufferCapacity();
        if (samples + queued > capacity) {
            printf("WARNING: samples: %i, queued: %i, capacity: %i (fps: %i)\n",
                   samples, queued, capacity, vdp_pal ? 50 : 60);
        }
#endif
        getAudio()->play(sound_buffer, samples,
                         vdp_pal ? Audio::SyncMode::Safe : Audio::SyncMode::None);
    }

    return UiEmu::onUpdate();
}

void PGENUiEmu::resizeVideo(bool isGameGear) {
    if (isGameGear) {
        video->setTextureRect({0, 0, 160, 144});
        video->setSize(160, 144);
        video->lock(&bitmap.data, &bitmap.pitch, {0, 0, 160, 144});
    } else {
        video->setTextureRect({0, 0, bitmap.viewport.w, bitmap.viewport.h});
        video->setSize((float) bitmap.viewport.w, (float) bitmap.viewport.h);
        video->lock(&bitmap.data, &bitmap.pitch, {0, 0, bitmap.viewport.w, bitmap.viewport.h});
    }
    video->updateScaling();
}

void PGENUiEmu::loadBios() {
    system_bios = 0;

    /* Genesis BOOT ROM support (2KB max) */
    char *boot_rom_data;
    memset(boot_rom, 0xFF, 0x800);
    size_t size = getUi()->getIo()->read(MD_BIOS, &boot_rom_data, 0x800);
    if (boot_rom_data && size == 0x800) {
        memcpy(boot_rom, boot_rom_data, 0x800);
        free(boot_rom_data);

        /* check BOOT ROM */
        if (!memcmp((char *) (boot_rom + 0x120), "GENESIS OS", 10)) {
            /* mark Genesis BIOS as loaded */
            printf("GENESIS BOOT ROM LOADED\n");
            system_bios = SYSTEM_MD;
        }

        /* Byteswap ROM */
        for (int i = 0; i < 0x800; i += 2) {
            uint8 temp = boot_rom[i];
            boot_rom[i] = boot_rom[i + 1];
            boot_rom[i + 1] = temp;
        }
    } else if (boot_rom_data) {
        free(boot_rom_data);
    }
}

void PGENUiEmu::loadBram() {
    if (system_hw != SYSTEM_MCD) {
        return;
    }

    char *data;
    std::string ramPath = ui->getIo()->getDataPath() + "rams/";
    size_t size = getUi()->getIo()->read(ramPath + Utility::removeExt(currentGame.path) + ".brm", &data, 0x2000);
    if (data && size == 0x2000) {
        memcpy(scd.bram, data, 0x2000);
        free(data);
        data = nullptr;
        /* update CRC */
        brm_crc[0] = crc32(0, scd.bram, 0x2000);
    } else {
        /* force internal backup RAM format (does not use previous region backup RAM) */
        scd.bram[0x1fff] = 0;
        if (data) {
            free(data);
            data = nullptr;
        }
    }

    /* check if internal backup RAM is correctly formatted */
    if (memcmp(scd.bram + 0x2000 - 0x20, brm_format + 0x20, 0x20) != 0) {
        /* clear internal backup RAM */
        memset(scd.bram, 0x00, 0x2000 - 0x40);

        /* internal Backup RAM size fields */
        brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] = 0x00;
        brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] = (sizeof(scd.bram) / 64) - 3;

        /* format internal backup RAM */
        memcpy(scd.bram + 0x2000 - 0x40, brm_format, 0x40);

        /* clear CRC to force file saving (in case previous region backup RAM was also formatted) */
        brm_crc[0] = 0;
    }

    /* automatically load cartridge backup RAM (if enabled) */
    if (scd.cartridge.id) {
        // TODO: read chunks
        size = getUi()->getIo()->read(ramPath + CD_BRAM_CART, &data, 0x810000);
        if (data && size == 0x810000) {
            memcpy(scd.cartridge.area, data, 0x810000);
            free(data);
            brm_crc[1] = crc32(0, scd.cartridge.area, scd.cartridge.mask + 1);
        } else if (data) {
            free(data);
        }

        /* check if cartridge backup RAM is correctly formatted */
        if (memcmp(scd.cartridge.area + scd.cartridge.mask + 1 - 0x20, brm_format + 0x20, 0x20) != 0) {
            /* clear cartridge backup RAM */
            memset(scd.cartridge.area, 0x00, scd.cartridge.mask + 1);

            /* Cartridge Backup RAM size fields */
            brm_format[0x10] = brm_format[0x12] = brm_format[0x14] = brm_format[0x16] =
                    (((scd.cartridge.mask + 1) / 64) - 3) >> 8;
            brm_format[0x11] = brm_format[0x13] = brm_format[0x15] = brm_format[0x17] =
                    (((scd.cartridge.mask + 1) / 64) - 3) & 0xff;

            /* format cartridge backup RAM */
            memcpy(scd.cartridge.area + scd.cartridge.mask + 1 - 0x40, brm_format, 0x40);
        }
    }
}

void PGENUiEmu::saveBram() {
    if (system_hw != SYSTEM_MCD) {
        return;
    }

    std::string ramPath = ui->getIo()->getDataPath() + "rams/";

    /* verify that internal backup RAM has been modified */
    if (crc32(0, scd.bram, 0x2000) != brm_crc[0]) {
        /* check if it is correctly formatted before saving */
        if (!memcmp(scd.bram + 0x2000 - 0x20, brm_format + 0x20, 0x20)) {
            ui->getIo()->write(ramPath + Utility::removeExt(currentGame.path) + ".brm", (const char *) scd.bram,
                               0x2000);
            /* update CRC */
            brm_crc[0] = crc32(0, scd.bram, 0x2000);
        }
    }

    /* verify that cartridge backup RAM has been modified */
    if (scd.cartridge.id && (crc32(0, scd.cartridge.area, scd.cartridge.mask + 1) != brm_crc[1])) {
        /* check if it is correctly formatted before saving */
        if (!memcmp(scd.cartridge.area + scd.cartridge.mask + 1 - 0x20, brm_format + 0x20, 0x20)) {
            ui->getIo()->write(ramPath + CD_BRAM_CART, (const char *) scd.cartridge.area, 0x810000);
            /* update CRC */
            brm_crc[1] = crc32(0, scd.cartridge.area, scd.cartridge.mask + 1);
        }
    }
}

//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "uiEmu.h"
#include "osd.h"

extern "C" {
#include "shared.h"
}

static short sound_buffer[2048];

PGENUiEmu::PGENUiEmu(UiMain *ui) : UiEmu(ui) {
    printf("PGENUiEmu()\n");
}

int PGENUiEmu::load(const ss_api::Game &game) {
    getUi()->getUiProgressBox()->setTitle(game.name);
    getUi()->getUiProgressBox()->setMessage("Please wait...");
    getUi()->getUiProgressBox()->setProgress(0);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Visible);
    getUi()->getUiProgressBox()->setLayer(1000);
    getUi()->flip();

    set_config_defaults();
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

    // video init
    memset(&bitmap, 0, sizeof(bitmap));
    bitmap.width = 320;
    bitmap.height = 224;
    bitmap.viewport.x = 0;
    bitmap.viewport.y = 0;
    bitmap.viewport.w = 320;
    bitmap.viewport.h = 224;
    addVideo((void **) &bitmap.data, &bitmap.pitch, {320, 224});

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
        // TODO
        // inputs
        auto *players = getUi()->getInput()->getPlayers();

        if (bitmap.viewport.changed & 1) {
            bitmap.viewport.changed &= ~1;
            printf("viewport changed: %ix%i (bitmap size: %ix%i)\n",
                   bitmap.viewport.w, bitmap.viewport.h,
                   bitmap.width, bitmap.height);
            if (bitmap.viewport.w != getVideo()->getTextureRect().width
                || bitmap.viewport.h != getVideo()->getTextureRect().height) {
                addVideo((void **) &bitmap.data, &bitmap.pitch, {bitmap.viewport.w, bitmap.viewport.h});
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
        getAudio()->play(sound_buffer, samples, vdp_pal);
    }

    return UiEmu::onUpdate();
}

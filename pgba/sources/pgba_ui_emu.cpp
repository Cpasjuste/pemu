//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "pgba_ui_emu.h"

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

    // TODO

    getUi()->getUiProgressBox()->setProgress(1);
    getUi()->flip();
    getUi()->delay(500);
    getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);

    return UiEmu::load(game);
}

void PGBAUiEmu::stop() {
    UiEmu::stop();
}

bool PGBAUiEmu::onInput(c2d::Input::Player *players) {
    return UiEmu::onInput(players);
}

void PGBAUiEmu::onUpdate() {
    if (!isPaused()) {
#if 0
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
#endif
    }

    return UiEmu::onUpdate();
}

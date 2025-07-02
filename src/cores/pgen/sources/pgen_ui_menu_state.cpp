//
// Created by cpasjuste on 28/09/18.
//

#include "skeleton/pemu.h"
#include "pgen_ui_menu_state.h"

extern "C" {
#include "shared.h"
}

bool PGENUIStateMenu::loadStateCore(const char *path) {
    bool ret = false;
    const auto buf = (uint8_t *) malloc(STATE_SIZE);
    const auto size = getUi()->getIo()->read(path, (char *) buf, STATE_SIZE);
    if (size == STATE_SIZE) {
        ret = state_load(buf);
    }

    if (buf) {
        free(buf);
    }

    return ret;
}

bool PGENUIStateMenu::saveStateCore(const char *path) {
    const auto buf = (uint8_t *) malloc(STATE_SIZE);
    bool ret = false;
    if (buf) {
        const auto len = state_save(buf);
        ret = getUi()->getIo()->write(path, (const char *) buf, len);
        free(buf);
    }

    return ret;
}

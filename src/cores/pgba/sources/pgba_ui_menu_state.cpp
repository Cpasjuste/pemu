//
// Created by cpasjuste on 28/09/18.
//

#include "skeleton/pemu.h"
#include "pgba_ui_menu_state.h"
extern "C" {
#include "mgba-util/vfs.h"
#include "mgba/core/serialize.h"
}

extern mCore *s_core;

bool PGBAUIStateMenu::loadStateCore(const char *path) {
    auto vf = VFileOpen(path, O_RDONLY);
    if (!vf) {
        printf("PGBAUIStateMenu::loadStateCore: could not open %s\n", path);
        return false;
    }

    bool success = mCoreLoadStateNamed(s_core, vf, SAVESTATE_RTC);
    vf->close(vf);

    printf("PGBAUIStateMenu::loadStateCore: success = %i\n", success);

    return success;
}

bool PGBAUIStateMenu::saveStateCore(const char *path) {
    auto vf = VFileOpen(path, O_CREAT | O_TRUNC | O_RDWR);
    if (!vf) {
        printf("PGBAUIStateMenu::saveStateCore: could not open %s\n", path);
        return false;
    }

    bool success = mCoreSaveStateNamed(s_core, vf, SAVESTATE_SAVEDATA | SAVESTATE_RTC | SAVESTATE_METADATA);
    vf->close(vf);

    if (!success) {
        getUi()->getIo()->removeFile(path);
    }

    printf("PGBAUIStateMenu::saveStateCore: success = %i\n", success);

    return success;
}

//
// Created by cpasjuste on 28/09/18.
//

#include "skeleton/pemu.h"
#include "psnes_ui_menu_state.h"
#include "snapshot.h"

bool PSNESUIStateMenu::loadStateCore(const char *path) {
    return S9xUnfreezeGame(path) == TRUE;
}

bool PSNESUIStateMenu::saveStateCore(const char *path) {
    return S9xFreezeGame(path) == TRUE;
}

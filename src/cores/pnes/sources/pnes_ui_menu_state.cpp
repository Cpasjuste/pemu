//
// Created by cpasjuste on 28/09/18.
//

#include "skeleton/pemu.h"
#include "pnes_ui_menu_state.h"

extern int nestopia_state_load(const char *path);

extern int nestopia_state_save(const char *path);

PNESUIStateMenu::PNESUIStateMenu(pemu::UiMain *ui) : pemu::UiMenuState(ui) {

}

bool PNESUIStateMenu::loadStateCore(const char *path) {
    return nestopia_state_load(path) == 0;
}

bool PNESUIStateMenu::saveStateCore(const char *path) {
    return nestopia_state_save(path) == 0;
}

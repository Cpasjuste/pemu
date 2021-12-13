//
// Created by cpasjuste on 28/09/18.
//

#include "c2dui.h"
#include "uiStateMenu.h"

extern int nestopia_state_load(const char *path);

extern int nestopia_state_save(const char *path);

PNESUIStateMenu::PNESUIStateMenu(c2dui::UiMain *ui) : c2dui::UiStateMenu(ui) {

}

bool PNESUIStateMenu::loadStateCore(const char *path) {
    return nestopia_state_load(path) == 0;
}

bool PNESUIStateMenu::saveStateCore(const char *path) {
    return nestopia_state_save(path) == 0;
}

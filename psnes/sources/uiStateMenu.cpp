//
// Created by cpasjuste on 28/09/18.
//

#include "c2dui.h"
#include "uiStateMenu.h"
#include "snapshot.h"

PSNESUIStateMenu::PSNESUIStateMenu(c2dui::UiMain *ui) : c2dui::UiStateMenu(ui) {

}

bool PSNESUIStateMenu::loadStateCore(const char *path) {
    return S9xUnfreezeGame(path) == TRUE;
}

bool PSNESUIStateMenu::saveStateCore(const char *path) {
    return S9xFreezeGame(path) == TRUE;
}

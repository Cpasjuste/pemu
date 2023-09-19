//
// Created by cpasjuste on 28/09/18.
//

#include "skeleton/pemu.h"
#include "pfbneo_ui_menu_state.h"

extern int BurnStateLoad(char *szName, int bAll, int (*pLoadGame)());

extern int BurnStateSave(char *szName, int bAll);

extern int DrvInitCallback();

PFBAUIStateMenu::PFBAUIStateMenu(pemu::UiMain *ui) : pemu::UiMenuState(ui) {

}

bool PFBAUIStateMenu::loadStateCore(const char *path) {
    return BurnStateLoad((char *) path, 1, &DrvInitCallback) == 0;
}

bool PFBAUIStateMenu::saveStateCore(const char *path) {
    return BurnStateSave((char *) path, 1) == 0;
}

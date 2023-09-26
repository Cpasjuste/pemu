//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PGEN_UI_STATE_MENU_H
#define PEMU_PGEN_UI_STATE_MENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PGENUIStateMenu : public pemu::UiMenuState {

public:
    explicit PGENUIStateMenu(pemu::UiMain *ui) : pemu::UiMenuState(ui) {};

    bool loadStateCore(const char *path) override;

    bool saveStateCore(const char *path) override;

};

#endif //PEMU_PGEN_UI_STATE_MENU_H

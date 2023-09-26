//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PNES_UI_STATE_MENU_H
#define PEMU_PNES_UI_STATE_MENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PNESUIStateMenu : public pemu::UiMenuState {
public:
    explicit PNESUIStateMenu(pemu::UiMain *ui) : pemu::UiMenuState(ui) {};

    bool loadStateCore(const char *path, void *data = nullptr) override;

    bool saveStateCore(const char *path, void *data = nullptr) override;

};

#endif //PEMU_PNES_UI_STATE_MENU_H

//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PSNES_UI_STATE_MENU_H
#define PEMU_PSNES_UI_STATE_MENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PSNESUIStateMenu : public pemu::UiMenuState {
public:
    explicit PSNESUIStateMenu(pemu::UiMain *ui) : pemu::UiMenuState(ui) {};

    bool loadStateCore(const char *path, void *data = nullptr) override;

    bool saveStateCore(const char *path, void *data = nullptr) override;

};

#endif //PEMU_PSNES_UI_STATE_MENU_H

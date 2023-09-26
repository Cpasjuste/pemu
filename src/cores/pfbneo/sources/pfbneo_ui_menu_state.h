//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PNESUISTATEMENU_H
#define PEMU_PNESUISTATEMENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PFBAUIStateMenu : public pemu::UiMenuState {

public:
    explicit PFBAUIStateMenu(pemu::UiMain *ui);

    bool loadStateCore(const char *path, void *data = nullptr) override;

    bool saveStateCore(const char *path, void *data = nullptr) override;

};

#endif //PEMU_PNESUISTATEMENU_H

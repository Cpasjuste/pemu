//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PSNESUISTATEMENU_H
#define PEMU_PSNESUISTATEMENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PSNESUIStateMenu : public pemu::UiMenuState {

public:
    PSNESUIStateMenu(pemu::UiMain *ui);

    bool loadStateCore(const char *path);

    bool saveStateCore(const char *path);

};

#endif //PEMU_PSNESUISTATEMENU_H

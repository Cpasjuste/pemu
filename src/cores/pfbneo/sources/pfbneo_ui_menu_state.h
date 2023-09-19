//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PNESUISTATEMENU_H
#define PEMU_PNESUISTATEMENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PFBAUIStateMenu : public pemu::UiMenuState {

public:
    PFBAUIStateMenu(pemu::UiMain *ui);

    bool loadStateCore(const char *path);

    bool saveStateCore(const char *path);

};

#endif //PEMU_PNESUISTATEMENU_H

//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PSNESUISTATEMENU_H
#define PEMU_PSNESUISTATEMENU_H

#include "ui_main.h"
#include "ui_menu_state.h"

class PSNESUIStateMenu : public c2dui::UiMenuState {

public:
    PSNESUIStateMenu(c2dui::UiMain *ui);

    bool loadStateCore(const char *path);

    bool saveStateCore(const char *path);

};

#endif //PEMU_PSNESUISTATEMENU_H

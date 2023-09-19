//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PGENUISTATEMENU_H
#define PEMU_PGENUISTATEMENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PGENUIStateMenu : public pemu::UiMenuState {

public:
    PGENUIStateMenu(pemu::UiMain *ui) : pemu::UiMenuState(ui) {};

    bool loadStateCore(const char *path);

    bool saveStateCore(const char *path);

};

#endif //PEMU_PGENUISTATEMENU_H

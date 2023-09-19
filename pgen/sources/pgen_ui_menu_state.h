//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PGENUISTATEMENU_H
#define PEMU_PGENUISTATEMENU_H

#include "ui_main.h"
#include "ui_menu_state.h"

class PGENUIStateMenu : public c2dui::UiMenuState {

public:
    PGENUIStateMenu(c2dui::UiMain *ui) : c2dui::UiMenuState(ui) {};

    bool loadStateCore(const char *path);

    bool saveStateCore(const char *path);

};

#endif //PEMU_PGENUISTATEMENU_H

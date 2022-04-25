//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PGENUISTATEMENU_H
#define PEMU_PGENUISTATEMENU_H

#include "c2dui_ui_main.h"
#include "c2dui_ui_menu_state.h"

class PGENUIStateMenu : public c2dui::UiStateMenu {

public:
    PGENUIStateMenu(c2dui::UiMain *ui);

    bool loadStateCore(const char *path);

    bool saveStateCore(const char *path);

};

#endif //PEMU_PGENUISTATEMENU_H

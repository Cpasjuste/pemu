//
// Created by cpasjuste on 28/09/18.
//

#ifndef PEMU_PGBA_UI_STATE_MENU_H
#define PEMU_PGBA_UI_STATE_MENU_H

#include "skeleton/ui_main.h"
#include "skeleton/ui_menu_state.h"

class PGBAUIStateMenu : public pemu::UiMenuState {
public:
    explicit PGBAUIStateMenu(pemu::UiMain *ui) : pemu::UiMenuState(ui) {};

    bool loadStateCore(const char *path) override;

    bool saveStateCore(const char *path) override;

};

#endif //PEMU_PGBA_UI_STATE_MENU_H

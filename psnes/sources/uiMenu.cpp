//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "uiMenu.h"

using namespace c2d;
using namespace c2dui;

PSNESUIMenu::PSNESUIMenu(c2dui::UiMain *ui) : UiMenu(ui) {

    printf("PSNESUIMenu()\n");
}

bool PSNESUIMenu::isOptionHidden(c2dui::Option *option) {

    if (!isRom() && option->getId() == Option::Id::ROM_PSNES_HIGH_RES) {
        return true;
    }

    return UiMenu::isOptionHidden(option);
}

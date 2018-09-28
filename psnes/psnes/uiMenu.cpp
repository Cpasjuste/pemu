//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "uiMenu.h"

using namespace c2d;
using namespace c2dui;

PSNESGuiMenu::PSNESGuiMenu(c2dui::UIMain *ui) : UIMenu(ui) {

    printf("PSNESGuiMenu()\n");
}

bool PSNESGuiMenu::isOptionHidden(c2dui::Option *option) {

    if (!isRom() && option->id == Option::Index::ROM_HIGH_RES) {
        return true;
    }

    return UIMenu::isOptionHidden(option);
}

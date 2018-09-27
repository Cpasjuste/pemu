//
// Created by cpasjuste on 01/06/18.
//

#include "c2dui.h"
#include "uiMenu.h"

using namespace c2d;
using namespace c2dui;

PNESGuiMenu::PNESGuiMenu(c2dui::C2DUIGuiMain *ui) : C2DUIGuiMenu(ui) {

    printf("PNESGuiMenu()\n");
}

bool PNESGuiMenu::isOptionHidden(c2dui::C2DUIOption *option) {

    return C2DUIGuiMenu::isOptionHidden(option);
}

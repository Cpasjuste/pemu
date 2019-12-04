//
// Created by cpasjuste on 01/06/18.
//

#include "burner.h"

#include "c2dui.h"
#include "uiMenu.h"

using namespace c2d;
using namespace c2dui;

PFBAGuiMenu::PFBAGuiMenu(c2dui::UIMain *ui) : UIMenu(ui) {

}

bool PFBAGuiMenu::isOptionHidden(c2dui::Option *option) {

    ss_api::Game game = getUi()->getUiRomList()->getSelection();

    if (isRom() && option->getId() == Option::Id::ROM_ROTATION
        && !game.id.empty() && game.rotation == "0") {
        return true;
    }

    // Neo-Geo system id == 142
    if (isRom() && option->getId() == Option::Id::ROM_NEOBIOS
        && !game.id.empty() && game.system.id != "142") {
        return true;
    }

#ifdef __SWITCH__
    if (option->getId() == Option::Id::ROM_FRAMESKIP) {
        return true;
    }
#endif

    return UIMenu::isOptionHidden(option);
}

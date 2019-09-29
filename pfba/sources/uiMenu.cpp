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

    if (isRom() && option->getId() == Option::Id::ROM_ROTATION && !game.id.empty() && !(game.rotation == "0")) {
        return true;
    }

    // TODO: sscrap - add hardware filtering
    //if (isRom() && option->getId() == Option::Id::ROM_NEOBIOS
    //    && rom != nullptr && !(getUi()->getUiRomList()->getRomList()->isHardware(rom->hardware, HARDWARE_PREFIX_SNK))) {
    //    return true;
    //}

#ifdef __SWITCH__
    if (option->getId() == Option::Id::ROM_FRAMESKIP) {
        return true;
    }
#endif

    return UIMenu::isOptionHidden(option);
}

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

    RomList::Rom *rom = getUi()->getUiRomList()->getSelection();

    if (isRom() && option->getId() == Option::Id::ROM_ROTATION
        && rom != nullptr && !(rom->flags & BDF_ORIENTATION_VERTICAL)) {
        return true;
    }

    if (isRom() && option->getId() == Option::Id::ROM_NEOBIOS
        && rom != nullptr && !(getUi()->getUiRomList()->getRomList()->isHardware(rom->hardware, HARDWARE_PREFIX_SNK))) {
        return true;
    }

#ifdef __SWITCH__
    if (option->getId() == Option::Index::ROM_FRAMESKIP) {
        return true;
    }
#endif

    return UIMenu::isOptionHidden(option);
}

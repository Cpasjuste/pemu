//
// Created by cpasjuste on 01/06/18.
//

#include "burner.h"

#include "c2dui.h"
#include "uiMenu.h"

using namespace c2d;
using namespace c2dui;

PFBAGuiMenu::PFBAGuiMenu(c2dui::C2DUIGuiMain *ui) : C2DUIGuiMenu(ui) {

}

bool PFBAGuiMenu::isOptionHidden(c2dui::C2DUIOption *option) {

    C2DUIRomList::Rom *rom = getUi()->getUiRomList()->getSelection();

    if (isRom() && option->index == C2DUIOption::Index::ROM_ROTATION
        && rom != nullptr && !(rom->flags & BDF_ORIENTATION_VERTICAL)) {
        return true;
    }

    if (isRom() && option->index == C2DUIOption::Index::ROM_NEOBIOS
        && rom != nullptr && !(getUi()->getUiRomList()->getRomList()->isHardware(rom->hardware, HARDWARE_PREFIX_SNK))) {
        return true;
    }

#ifdef __SWITCH__
    if (option->index == C2DUIOption::Index::ROM_FRAMESKIP) {
        return true;
    }
#endif

    return C2DUIGuiMenu::isOptionHidden(option);
}

//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include <vector>

#include "c2dui.h"
#include "pnes_config.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

PNESConfig::PNESConfig(Renderer *renderer, const std::string &home, int version)
        : C2DUIConfig(home, version) {

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();


    // hide "show working/available when no database is used...
    if (getValue(C2DUIOption::GUI_USE_DATABASE) == 0) {
        hide(C2DUIOption::GUI_SHOW_ALL);
        hide(C2DUIOption::GUI_SHOW_CLONES);
    }
}

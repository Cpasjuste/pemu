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

PNESConfig::PNESConfig(const std::string &home, int version)
        : Config(home, version) {

    if (!get(Option::GUI_USE_DATABASE)->getValueBool()) {
        hide(Option::GUI_SHOW_CLONES);
    }

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include <vector>

#include "c2dui.h"
#include "pnes_config.h"

using namespace c2d;
using namespace c2dui;

PNESConfig::PNESConfig(c2d::Io *io, int version) : Config(io, version) {

    //hide(Option::GUI_USE_DATABASE);
    //hide(Option::GUI_SHOW_CLONES);

    // set default rom options
    //reset();

    // load/overwrite configuration from file
    //load();
}

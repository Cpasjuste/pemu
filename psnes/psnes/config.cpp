//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include <vector>

#include "c2dui.h"
#include "config.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

PSNESConfig::PSNESConfig(const std::string &home, int version)
        : Config(home, version) {

    if (!get(Option::GUI_USE_DATABASE)->getValueBool()) {
        hide(Option::GUI_SHOW_CLONES);
    } else {
        // add "WORKING" to "SHOW_ALL" option
        std::vector<std::string> *values = get(Option::Index::GUI_SHOW_ALL)->getValues();
        values->insert(values->begin() + 1, "WORKING");
    }

    add(Option::Index::ROM_SHOW_FPS, "HIGH_RES", {"OFF", "ON"}, 0, Option::Index::ROM_HIGH_RES);
    get()->at(get()->size() - 1).setInfo(
            "Only enable high resolution mode for games which can use it,\n"
            "for example \"Secret Of Mana\".\n"
            "It does have some performance impact.\n\n"
            "Need a restart...");

    add(Option::Index::ROM_HIGH_RES, "CHEATS", {"OFF", "ON"}, 1, Option::Index::ROM_CHEATS);

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

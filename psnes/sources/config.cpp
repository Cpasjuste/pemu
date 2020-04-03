//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "config.h"

using namespace c2d;
using namespace c2dui;

PSNESConfig::PSNESConfig(c2d::Io *io, int version) : Config(io, version) {

    printf("PSNESConfig(%s, v%i)\n", getConfigPath().c_str(), version);

    add(Option::Id::ROM_SHOW_FPS, "HIGH_RES", {"OFF", "ON"}, 0, Option::Id::ROM_HIGH_RES, Option::Flags::BOOLEAN);
    get()->at(get()->size() - 1).setInfo(
            "Only enable high resolution mode for games which can use it,\n"
            "for example \"Secret Of Mana\".\n"
            "It does have some performance impact.\n\n"
            "Need a restart...");

    add(Option::Id::ROM_HIGH_RES, "CHEATS", {"OFF", "ON"}, 1, Option::Id::ROM_CHEATS, Option::Flags::BOOLEAN);

    // "c2dui_romlist" will also reload config, but we need new roms paths
    reset();
    load();
}

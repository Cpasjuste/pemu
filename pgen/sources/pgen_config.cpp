//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "pgen_config.h"

using namespace c2d;
using namespace c2dui;

PGENConfig::PGENConfig(c2d::Io *io, int version, const std::string &defaultRomsPath)
        : Config(io, version, defaultRomsPath) {
    // add default roms paths
    roms_paths.emplace_back(io->getDataPath() + "sms/");
    roms_paths.emplace_back(io->getDataPath() + "gamegear/");
    roms_paths.emplace_back(io->getDataPath() + "megacd/");
    roms_paths.emplace_back(io->getDataPath() + "sg1000/");

    // no need for auto-scaling mode on pgen
    get(Option::Id::ROM_SCALING_MODE)->set(
            {"SCALING_MODE", {"ASPECT", "INTEGER"}, 1,
             Option::Id::ROM_SCALING_MODE, Option::Flags::STRING});

    // "c2dui_romlist" will also reload config, but we need new roms paths
    reset();
    load();
}

//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "pgen_config.h"

using namespace c2d;
using namespace c2dui;

PGENConfig::PGENConfig(UiMain *ui, int version, const std::string &defaultRomsPath)
        : Config(ui, version, defaultRomsPath) {
    // add default roms paths
    roms_paths.emplace_back(ui->getIo()->getDataPath() + "sms/");
    roms_paths.emplace_back(ui->getIo()->getDataPath() + "gamegear/");
    roms_paths.emplace_back(ui->getIo()->getDataPath() + "megacd/");
#if 0
    roms_paths.emplace_back(ui->getIo()->getDataPath() + "sg1000/");
#endif
    // no need for auto-scaling mode on pgen
    get(Option::Id::ROM_SCALING_MODE)->set(
            {"SCALING_MODE", {"ASPECT", "INTEGER"}, 0,
             Option::Id::ROM_SCALING_MODE, Option::Flags::STRING});

    // "c2dui_romlist" will also reload config, but we need new roms paths
    reset();
    load();
}

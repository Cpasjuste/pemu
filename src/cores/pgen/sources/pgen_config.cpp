//
// Created by cpasjuste on 29/05/18.
//

#include "skeleton/pemu.h"
#include "pgen_config.h"

using namespace c2d;
using namespace pemu;

PGENConfig::PGENConfig(c2d::Io *io, int version) : PEMUConfig(io, "PGEN", version) {
    // change default rom path name in config file
    getGroup(CFG_ID_ROMS)->getOptions()->at(0).setName("MEGADRIVE");
    getGroup(CFG_ID_ROMS)->getOptions()->at(0).setString(io->getDataPath() + "megadrive/");
    // add pgen roms paths
    getGroup(CFG_ID_ROMS)->addOption({"MASTERSYSTEM", io->getDataPath() + "mastersystem/"});
    getGroup(CFG_ID_ROMS)->addOption({"GAMEGEAR", io->getDataPath() + "gamegear/"});
    getGroup(CFG_ID_ROMS)->addOption({"MEGACD", io->getDataPath() + "megacd/"});
    getGroup(CFG_ID_ROMS)->addOption({"SG1000", io->getDataPath() + "sg1000/"});

    // no need for auto-scaling mode
    getOption(PEMUConfig::Id::ROM_SCALING_MODE)->setArray({"ASPECT", "INTEGER"}, 0);

    // "c2dui_romlist" will also reload config, but we need new roms paths
    load();
}

//
// Created by cpasjuste on 29/05/18.
//

#include "skeleton/pemu.h"
#include "pgen_config.h"

using namespace c2d;
using namespace pemu;

PGENConfig::PGENConfig(c2d::Io *io, int version) : PEMUConfig(io, "PGEN", version) {
    // no need for auto-scaling mode
    getOption(PEMUConfig::OptId::EMU_SCALING_MODE)->setArray({"ASPECT", "INTEGER"}, 0);

    // "romlist.cpp" (RomList::build) will also reload config, but we need new roms paths
    PEMUConfig::load();

    // add custom rom path
    PEMUConfig::addRomPath("MEGADRIVE", io->getDataPath() + "megadrive/", {1, 0, "Megadrive"});
    PEMUConfig::addRomPath("MASTERSYSTEM", io->getDataPath() + "mastersystem/", {2, 0, "Master System"});
    PEMUConfig::addRomPath("GAMEGEAR", io->getDataPath() + "gamegear/", {21, 0, "Game Gear"});
    PEMUConfig::addRomPath("MEGACD", io->getDataPath() + "megacd/", {20, 0, "Mega-CD"});
    PEMUConfig::addRomPath("SG1000", io->getDataPath() + "sg1000/", {109, 0, "SG-1000"});
    PEMUConfig::save();

    // create roms paths if needed
    auto paths = getRomPaths();
    for (const auto &path: paths) {
        io->create(path.path);
    }
}

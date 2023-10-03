//
// Created by cpasjuste on 29/05/18.
//

#include "skeleton/pemu.h"
#include "pgba_config.h"

using namespace c2d;
using namespace pemu;

PGBAConfig::PGBAConfig(c2d::Io *io, int version) : PEMUConfig(io, "PGBA", version) {
    // no need for auto-scaling mode
    getOption(PEMUConfig::OptId::EMU_SCALING_MODE)->setArray({"ASPECT", "INTEGER"}, 0);

    // "romlist.cpp" (RomList::build) will also reload config, but we need new roms paths
    PEMUConfig::load();

    // add custom rom path
    PEMUConfig::addRomPath("GBA", io->getDataPath() + "roms/", {12, 0, "Game Boy Advance"});
    PEMUConfig::save();

    // create roms paths if needed
    auto paths = getRomPaths();
    for (const auto &path: paths) {
        io->create(path.path);
    }
}

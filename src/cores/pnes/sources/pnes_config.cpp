//
// Created by cpasjuste on 29/05/18.
//

#include "skeleton/pemu.h"
#include "pnes_config.h"

using namespace c2d;
using namespace pemu;

PNESConfig::PNESConfig(c2d::Io *io, int version) : PEMUConfig(io, "PNES", version) {
    printf("PNESConfig(%s, v%i)\n", getPath().c_str(), version);

    // no need for auto-scaling mode on pnes
    getOption(PEMUConfig::OptId::EMU_SCALING_MODE)->setArray({"ASPECT", "INTEGER"}, 0);

#ifdef __SWITCH__
    // on nintendo switch invert A/B buttons
    getOption(PEMUConfig::OptId::JOY_A)->setInteger(KEY_JOY_B_DEFAULT);
    getOption(PEMUConfig::OptId::JOY_B)->setInteger(KEY_JOY_A_DEFAULT);
#endif

    // "romlist.cpp" (RomList::build) will also reload config, but we need new roms paths
    PEMUConfig::load();

    // add custom rom path
    PEMUConfig::addRomPath("NES", io->getDataPath() + "roms/", {3, 0, "NES"});
    PEMUConfig::save();

    // create roms paths if needed
    auto paths = getRomPaths();
    for (const auto &path: paths) {
        io->create(path.path);
    }
}

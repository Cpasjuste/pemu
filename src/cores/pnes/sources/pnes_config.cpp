//
// Created by cpasjuste on 29/05/18.
//

#include "skeleton/pemu.h"
#include "pnes_config.h"

using namespace c2d;
using namespace pemu;

PNESConfig::PNESConfig(c2d::Io *io, int version) : PEMUConfig(io, "PNES", version) {
    printf("PNESConfig(%s, v%i)\n", getPath().c_str(), version);

    // add custom roms paths to config
    for (const auto &gl: PNESConfig::getCoreGameListInfo()) {
        getGroup(CFG_ID_ROMS)->addOption({gl.cfg_name, io->getDataPath() + gl.rom_path + "/"});
    }

    // no need for auto-scaling mode on pnes
    getOption(PEMUConfig::OptId::EMU_SCALING_MODE)->setArray({"ASPECT", "INTEGER"}, 0);

#ifdef __SWITCH__
    // on nintendo switch invert A/B buttons
    getOption(PEMUConfig::OptId::JOY_A)->setInteger(KEY_JOY_B_DEFAULT);
    getOption(PEMUConfig::OptId::JOY_B)->setInteger(KEY_JOY_A_DEFAULT);
#endif

    // "c2dui_romlist" will also reload config, but we need new roms paths
    load();
}

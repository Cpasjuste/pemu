//
// Created by cpasjuste on 29/05/18.
//

#include "skeleton/pemu.h"
#include "pgen_config.h"

using namespace c2d;
using namespace pemu;

PGENConfig::PGENConfig(c2d::Io *io, int version) : PEMUConfig(io, "PGEN", version) {
    // add custom roms paths to config
    for (const auto &gl: getCoreGameListInfo()) {
        getGroup(CFG_ID_ROMS)->addOption({gl.cfg_name, io->getDataPath() + gl.rom_path + "/"});
    }

    // no need for auto-scaling mode
    getOption(PEMUConfig::Id::ROM_SCALING_MODE)->setArray({"ASPECT", "INTEGER"}, 0);

    // "c2dui_romlist" will also reload config, but we need new roms paths
    load();
}

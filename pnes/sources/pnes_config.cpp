//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "pnes_config.h"

using namespace c2d;
using namespace c2dui;

PNESConfig::PNESConfig(c2d::Io *io, int version) : Config(io, version) {
    // no need for auto-scaling mode on pnes
#ifdef __3DS__
    get(Option::Id::ROM_SCALING_MODE)->set(
            {"SCALING_MODE", {"ASPECT", "INTEGER"}, 0,
             Option::Id::ROM_SCALING_MODE, Option::Flags::STRING});
#else
    get(Option::Id::ROM_SCALING_MODE)->set(
            {"SCALING_MODE", {"ASPECT", "INTEGER"}, 1,
             Option::Id::ROM_SCALING_MODE, Option::Flags::STRING});
#endif
    // "c2dui_romlist" will also reload config, but we need new roms paths
    reset();
    load();
}

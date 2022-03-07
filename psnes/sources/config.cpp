//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "config.h"

using namespace c2d;
using namespace c2dui;

PSNESConfig::PSNESConfig(c2d::Io *io, int version) : Config(io, version) {

    printf("PSNESConfig(%s, v%i)\n", getConfigPath().c_str(), version);

    add(Option::Id::ROM_SHOW_FPS, "HIGH_RES", {"OFF", "ON"}, 0,
        Option::Id::ROM_PSNES_HIGH_RES, Option::Flags::BOOLEAN);
    get(Option::Id::ROM_PSNES_HIGH_RES)->setInfo(
            "ENABLE HIGH RES FOR GAMES LIKE SECRET OF MANA - NEEDS A RESTART");

    add(Option::Id::ROM_PSNES_HIGH_RES, "CHEATS", {"OFF", "ON"}, 1,
        Option::Id::ROM_PSNES_CHEATS, Option::Flags::BOOLEAN);

    add(Option::Id::ROM_PSNES_CHEATS, "BLOCk_INVALID_VRAM", {"OFF", "ON"}, 1,
        Option::Id::ROM_PSNES_BLOCK_VRAM, Option::Flags::BOOLEAN);

    add(Option::Id::ROM_PSNES_BLOCK_VRAM, "TRANSPARENCY", {"OFF", "ON"}, 1,
        Option::Id::ROM_PSNES_TRANSPARENCY, Option::Flags::BOOLEAN);

    add(Option::Id::ROM_PSNES_TRANSPARENCY, "DISPLAY_MESSAGES", {"OFF", "ON"}, 1,
        Option::Id::ROM_PSNES_DISPLAY_MESSAGES, Option::Flags::BOOLEAN);

    add(Option::Id::ROM_PSNES_DISPLAY_MESSAGES, "FRAMESKIP",
        {"OFF", "AUTO", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, 0,
        Option::Id::ROM_PSNES_FRAMESKIP, Option::Flags::STRING);
#ifdef __VITA__
    get(Option::Id::ROM_PSNES_FRAMESKIP)->setValueString("2");
#endif

    add(Option::Id::ROM_PSNES_FRAMESKIP, "TURBO_MODE", {"OFF", "ON"}, 0,
        Option::Id::ROM_PSNES_TURBO_MODE, Option::Flags::BOOLEAN);

    add(Option::Id::ROM_PSNES_TURBO_MODE, "TURBO_FRAMESKIP",
        {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
         "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25"}, 15,
        Option::Id::ROM_PSNES_TURBO_FRAMESKIP, Option::Flags::STRING);

    // no need for auto-scaling mode on psnes
    get(Option::Id::ROM_SCALING_MODE)->set(
            {"SCALING_MODE", {"ASPECT", "INTEGER"}, 1,
             Option::Id::ROM_SCALING_MODE, Option::Flags::STRING});

    // "c2dui_romlist" will also reload config, but we need new roms paths
    reset();
    load();
}

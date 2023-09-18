//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "config.h"

using namespace c2d;
using namespace c2dui;

PSNESConfig::PSNESConfig(c2d::Io *io, int version) : PEMUConfig(io, "PSNES", version) {
    printf("PSNESConfig(%s, v%i)\n", getPath().c_str(), version);

    auto group = getGroup(PEMUConfig::Id::MENU_ROM_OPTIONS);
    if (!group) {
        printf("PSNESConfig: error, group not found (MENU_ROM_OPTIONS)\n");
        return;
    }

    group->addOption({"AUDIO_SYNC", {"OFF", "ON"}, 0, PEMUConfig::Id::ROM_AUDIO_SYNC,
                      "ON: PERFECT AUDIO - OFF: MINOR AUDIO STUTTERING (FAVOR FPS)"});
    group->addOption({"CHEATS", {"OFF", "ON"}, 1, PEMUConfig::Id::ROM_PSNES_CHEATS});
    group->addOption({"BLOCk_INVALID_VRAM", {"OFF", "ON"}, 1, PEMUConfig::Id::ROM_PSNES_BLOCK_VRAM});
    group->addOption({"TRANSPARENCY", {"OFF", "ON"}, 1, PEMUConfig::Id::ROM_PSNES_TRANSPARENCY});
    group->addOption({"DISPLAY_MESSAGES", {"OFF", "ON"}, 1, PEMUConfig::Id::ROM_PSNES_DISPLAY_MESSAGES});
    group->addOption({"FRAMESKIP",
                      {"OFF", "AUTO", "1", "2", "3", "4", "5", "6", "7", "8", "9"},
                      0, PEMUConfig::Id::ROM_PSNES_FRAMESKIP});
#ifdef __VITA__
    get(ConfigNew::Id::ROM_PSNES_FRAMESKIP)->setArrayIndex(3);
#endif
    group->addOption({"TURBO_MODE", {"OFF", "ON"}, 0, PEMUConfig::Id::ROM_PSNES_TURBO_MODE});
    group->addOption({"TURBO_FRAMESKIP",
                      {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                       "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25"},
                      15, PEMUConfig::Id::ROM_PSNES_TURBO_FRAMESKIP});

    // no need for auto-scaling mode
    getOption(PEMUConfig::Id::ROM_SCALING_MODE)->setArray({"ASPECT", "INTEGER"}, 0);

    // "c2dui_romlist" will also reload config, but we need new roms paths
    load();
}

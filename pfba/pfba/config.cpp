//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include <vector>

#include "burner.h"

#include "c2dui.h"
#include "config.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

PFBAConfig::PFBAConfig(const std::string &home, int version)
        : Config(home, version) {

    // add fba default roms paths
    getRomPaths()->emplace_back(home + "coleco/");
    getRomPaths()->emplace_back(home + "gamegear/");
    getRomPaths()->emplace_back(home + "megadriv/");
    getRomPaths()->emplace_back(home + "msx/");
    getRomPaths()->emplace_back(home + "pce/");
    getRomPaths()->emplace_back(home + "sg1000/");
    getRomPaths()->emplace_back(home + "sgx/");
    getRomPaths()->emplace_back(home + "sms/");
    getRomPaths()->emplace_back(home + "tg16/");
    for (size_t i = getRomPaths()->size(); i < C2DUI_ROMS_PATHS_MAX; i++) {
        getRomPaths()->emplace_back(std::string());
    }

    // add fba hardware list
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SNK, "Neo Geo");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CAPCOM, "CPS-1");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CPS2, "CPS-2");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CPS3, "CPS-3");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_IGS_PGM, "PGM");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_PSIKYO, "Psikyo");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CAVE, "Cave");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA, "Sega");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CAPCOM_MISC, "Capcom (Other)");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_DATAEAST, "Data East");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_GALAXIAN, "Galaxian");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_IREM, "Irem");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_KANEKO, "Kaneko");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_KONAMI, "Konami");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MISC_PRE90S, "Misc (pre 90s)");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MISC_POST90S, "Misc (post 90s)");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_PACMAN, "Pacman");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SETA, "Seta");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_TAITO, "Taito");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_TECHNOS, "Technos");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_TOAPLAN, "Toaplan");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_MEGADRIVE, "Sega MegaDrive");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, "Sega MasterSystem");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_SG1000, "Sega SG1000");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_GAME_GEAR, "Sega GameGear");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_PCENGINE, "PC-Engine");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_COLECO, "Coleco");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MIDWAY, "Midway");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MSX, "MSX");
    std::vector<std::string> hardware_names;
    for (auto &i : *getHardwareList()) {
        hardware_names.emplace_back(i.name);
    }

    ////////////////////////////////////////////////////////////
    /// main/gui config
    ////////////////////////////////////////////////////////////

    hide(Option::GUI_USE_DATABASE);

#ifdef __PSP2__
    add(Option::Index::ROM_SHOW_FPS, "ROTATION",
        {"OFF", "ON", "FLIP", "CAB MODE"}, 0, Option::Index::ROM_ROTATION);
#else
    add(Option::Index::ROM_SHADER, "ROTATION",
        {"OFF", "ON", "FLIP"}, 0, Option::Index::ROM_ROTATION);
#endif
    add(Option::Index::ROM_ROTATION, "NEOBIOS",
        {"UNIBIOS_3_2", "AES_ASIA", "AES_JPN", "DEVKIT", "MVS_ASIA_EUR_V6S1",
         "MVS_ASIA_EUR_V5S1", "MVS_ASIA_EUR_V3S4", "MVS_USA_V5S2",
         "MVS_USA_V5S4", "MVS_USA_V5S6", "MVS_JPN_V6", "MVS_JPN_V5",
         "MVS_JPN_V3S4", "NEO_MVH_MV1C", "MVS_JPN_J3", "DECK_V6"},
        0, Option::Index::ROM_NEOBIOS);

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

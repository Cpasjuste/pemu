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

    printf("PFBAConfig(%s, v%i)\n", home.c_str(), version);

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
    for (size_t i = getRomPaths()->size(); i < 20; i++) {
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
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SPECTRUM, "Spectrum");
    std::vector<std::string> hardware_names;
    for (auto &i : *getHardwareList()) {
        hardware_names.emplace_back(i.name);
    }

    ////////////////////////////////////////////////////////////
    /// pfba custom config
    ////////////////////////////////////////////////////////////

    /// UI OPTIONS
    hide(Option::GUI_USE_DATABASE);

    // add "WORKING" to "SHOW_ALL" option
    std::vector<std::string> *values = get(Option::Id::GUI_SHOW_ALL)->getValues();
    values->insert(values->begin() + 1, "WORKING");

    add(Option::Id::GUI_SHOW_CLONES, "SHOW_HARDWARE",
        hardware_names, 0, Option::Id::GUI_SHOW_HARDWARE);

    /// ROMS OPTIONS
    add(Option::Id::ROM_SHADER, "FORCE_60HZ",
        {"OFF", "ON"}, 0, Option::Id::ROM_FORCE_60HZ);
    add(Option::Id::ROM_FORCE_60HZ, "FORCE_50HZ",
        {"OFF", "ON"}, 0, Option::Id::ROM_FORCE_50HZ);

    // audio
    add(Option::Id::ROM_FORCE_50HZ, "FORCE_AUDIO_SYNC",
        {"OFF", "ON"}, 1, Option::Id::ROM_AUDIO_SYNC);
    add(Option::Id::ROM_AUDIO_SYNC, "AUDIO_FREQUENCY",
        {"11025", "22050", "32000", "44100", "48000"}, 3, Option::Id::ROM_AUDIO_FREQ);
    add(Option::Id::ROM_AUDIO_SYNC, "AUDIO_INTERPOLATION",
        {"0", "1", "3"}, 2, Option::Id::ROM_AUDIO_INTERPOLATION);
    add(Option::Id::ROM_AUDIO_INTERPOLATION, "AUDIO_FM_INTERPOLATION",
        {"0", "1", "3"}, 2, Option::Id::ROM_AUDIO_FMINTERPOLATION);
#ifdef __PSP2__
    add(Option::Index::ROM_AUDIO_FMINTERPOLATION, "ROTATION",
        {"OFF", "ON", "FLIP", "CAB MODE"}, 0, Option::Index::ROM_ROTATION);
#else
    add(Option::Id::ROM_AUDIO_FMINTERPOLATION, "ROTATION",
        {"OFF", "ON", "FLIP"}, 0, Option::Id::ROM_ROTATION);
#endif
    add(Option::Id::ROM_ROTATION, "NEOBIOS",
        {"UNIBIOS_3_3", "UNIBIOS_3_2", "UNIBIOS_3_1",
         "MVS_ASIA_EUR_V6S1", "MVS_ASIA_EUR_V5S1", "MVS_ASIA_EUR_V3S4",
         "MVS_USA_V5S2", "MVS_USA_V5S4", "MVS_USA_V5S6",
         "MVS_JPN_V6", "MVS_JPN_V5", "MVS_JPN_V3S4", "MVS_JPN_J3",
         "AES_ASIA", "AES_JAPAN",
         "NEO_MVH_MV1CA", "NEO_MVH_MV1CJ",
         "DECK_V6", "DEVKIT"},
        2, Option::Id::ROM_NEOBIOS);

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

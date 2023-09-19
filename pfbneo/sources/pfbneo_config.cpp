//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include "pemu.h"
#include "pfbneo_config.h"

using namespace c2d;
using namespace c2dui;

#define C2D_CONFIG_RESTART_EMU_NEEDED "YOU NEED TO RESTART EMULATION AFTER CHANGING THIS OPTION"

PFBAConfig::PFBAConfig(c2d::Io *io, int version) : PEMUConfig(io, "PFBNEO", version) {
    printf("PFBNConfig(%s, v%i)\n", getPath().c_str(), version);

    // change default rom path name in config file
    getGroup(CFG_ID_ROMS)->getOptions()->at(0).setName("ARCADE");
    getGroup(CFG_ID_ROMS)->getOptions()->at(0).setString(io->getDataPath() + "arcade/");
#ifndef __PFBN_LIGHT__
    // add fbneo consoles paths
    getGroup(CFG_ID_ROMS)->addOption({"CHANNELF", io->getDataPath() + "channelf/"});
    getGroup(CFG_ID_ROMS)->addOption({"COLECO", io->getDataPath() + "coleco/"});
    getGroup(CFG_ID_ROMS)->addOption({"FDS", io->getDataPath() + "fds/"});
    getGroup(CFG_ID_ROMS)->addOption({"GAMEGEAR", io->getDataPath() + "gamegear/"});
    getGroup(CFG_ID_ROMS)->addOption({"MEGADRIVE", io->getDataPath() + "megadrive/"});
    getGroup(CFG_ID_ROMS)->addOption({"MSX", io->getDataPath() + "msx/"});
    getGroup(CFG_ID_ROMS)->addOption({"NES", io->getDataPath() + "nes/"});
    getGroup(CFG_ID_ROMS)->addOption({"NGP", io->getDataPath() + "ngp/"});
    getGroup(CFG_ID_ROMS)->addOption({"PCE", io->getDataPath() + "pce/"});
    getGroup(CFG_ID_ROMS)->addOption({"SG1000", io->getDataPath() + "sg1000/"});
    getGroup(CFG_ID_ROMS)->addOption({"SGX", io->getDataPath() + "sgx/"});
    getGroup(CFG_ID_ROMS)->addOption({"SMS", io->getDataPath() + "sms/"});
    getGroup(CFG_ID_ROMS)->addOption({"SPECTRUM", io->getDataPath() + "spectrum/"});
    getGroup(CFG_ID_ROMS)->addOption({"TG16", io->getDataPath() + "tg16/"});
#endif

    ////////////////////////////////////////////////////////////
    /// pfbneo custom config
    ////////////////////////////////////////////////////////////

    /// MAIN OPTIONS
    get(PEMUConfig::Id::GUI_SHOW_ZIP_NAMES)->setArrayIndex(0);

    /// ROMS OPTIONS
    auto group = getGroup(PEMUConfig::Id::MENU_ROM_OPTIONS);
    if (!group) {
        printf("PFBNConfig: error, group not found (MENU_ROM_OPTIONS)\n");
        return;
    }
    group->addOption({"FORCE_60HZ", {"OFF", "ON"}, 1, PEMUConfig::Id::ROM_FORCE_60HZ, C2D_CONFIG_RESTART_EMU_NEEDED});
    group->addOption({"AUDIO_FREQUENCY", {"11025", "22050", "32000", "44100", "48000"},
                      3, PEMUConfig::Id::ROM_AUDIO_FREQ, C2D_CONFIG_RESTART_EMU_NEEDED});
    group->addOption({"AUDIO_INTERPOLATION", {"0", "1", "3"},
                      2, PEMUConfig::Id::ROM_AUDIO_INTERPOLATION, C2D_CONFIG_RESTART_EMU_NEEDED});
    group->addOption({"AUDIO_FM_INTERPOLATION", {"0", "1", "3"},
                      2, PEMUConfig::Id::ROM_AUDIO_FMINTERPOLATION, C2D_CONFIG_RESTART_EMU_NEEDED});
#ifdef __VITA__
    group->addOption({"ROTATION", {"OFF", "ON", "FLIP", "CAB MODE"},
                      1, PEMUConfig::Id::ROM_ROTATION, C2D_CONFIG_RESTART_EMU_NEEDED});
#else
    group->addOption({"ROTATION", {"OFF", "ON", "FLIP"},
                      1, PEMUConfig::Id::ROM_ROTATION, C2D_CONFIG_RESTART_EMU_NEEDED});
#endif
    group->addOption(
            {"NEOBIOS", {"UNIBIOS_4_0", "UNIBIOS_3_3", "UNIBIOS_3_2", "UNIBIOS_3_1",
                         "MVS_ASIA_EUR_V6S1", "MVS_ASIA_EUR_V5S1", "MVS_ASIA_EUR_V3S4",
                         "MVS_USA_V5S2", "MVS_USA_V5S4", "MVS_USA_V5S6", "MVS_JPN_V6",
                         "MVS_JPN_V5", "MVS_JPN_V3S4", "MVS_JPN_J3", "AES_ASIA",
                         "AES_JAPAN", "NEO_MVH_MV1CA", "NEO_MVH_MV1CJ", "DECK_V6", "DEVKIT"},
             0, PEMUConfig::Id::ROM_NEOBIOS, C2D_CONFIG_RESTART_EMU_NEEDED});
#ifdef __PFBA_ARM__
    // do not use unibios as default on vita for cyclone asm compatibility
    group->getOption(PEMUConfig::Id::ROM_NEOBIOS)->setArrayIndex(4);
    group->addOption({"FRAMESKIP", {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"},
                      0, PEMUConfig::Id::ROM_FRAMESKIP, C2D_CONFIG_RESTART_EMU_NEEDED});
#endif

#if defined(__PS4__) || defined(ANDROID)
    // force 48000hz audio output
    get(PEMUConfig::Id::ROM_AUDIO_FREQ)->setArrayIndex(4);
    get(PEMUConfig::Id::ROM_AUDIO_FREQ)->setFlags(PEMUConfig::Flags::HIDDEN);
#endif

    // "c2dui_romlist" will also reload config, but we need new roms paths
    load();
}

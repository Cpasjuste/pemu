//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include "skeleton/pemu.h"
#include "pfbneo_config.h"

using namespace c2d;
using namespace pemu;

#define C2D_CONFIG_RESTART_EMU_NEEDED "YOU NEED TO RESTART EMULATION AFTER CHANGING THIS OPTION"

PFBAConfig::PFBAConfig(c2d::Io *io, int version) : PEMUConfig(io, "PFBNEO", version) {
    printf("PFBNConfig(%s, v%i)\n", getPath().c_str(), version);

    /// MAIN OPTIONS
    get(PEMUConfig::OptId::UI_SHOW_ZIP_NAMES)->setArrayIndex(0);

    /// ROMS OPTIONS
    auto group = getGroup(PEMUConfig::GrpId::EMULATION);
    if (!group) {
        printf("PFBNConfig: error, group not found (MENU_ROM_OPTIONS)\n");
        return;
    }
    group->addOption(
            {"FORCE_60HZ", {"OFF", "ON"}, 1, PEMUConfig::OptId::EMU_FORCE_60HZ, C2D_CONFIG_RESTART_EMU_NEEDED});
    group->addOption({"AUDIO_FREQUENCY", {"11025", "22050", "32000", "44100", "48000"},
                      3, PEMUConfig::OptId::EMU_AUDIO_FREQ, C2D_CONFIG_RESTART_EMU_NEEDED});
    group->addOption({"AUDIO_INTERPOLATION", {"0", "1", "3"},
                      2, PEMUConfig::OptId::EMU_AUDIO_INTERPOLATION, C2D_CONFIG_RESTART_EMU_NEEDED});
    group->addOption({"AUDIO_FM_INTERPOLATION", {"0", "1", "3"},
                      2, PEMUConfig::OptId::EMU_AUDIO_FMINTERPOLATION, C2D_CONFIG_RESTART_EMU_NEEDED});
#ifdef __VITA__
    group->addOption({"ROTATION", {"OFF", "ON", "FLIP", "CAB MODE"},
                      1, PEMUConfig::OptId::EMU_ROTATION, C2D_CONFIG_RESTART_EMU_NEEDED});
#else
    group->addOption({"ROTATION", {"OFF", "ON", "FLIP"},
                      1, PEMUConfig::OptId::EMU_ROTATION, C2D_CONFIG_RESTART_EMU_NEEDED});
#endif
    group->addOption(
            {"NEOBIOS", {"UNIBIOS_4_0", "UNIBIOS_3_3", "UNIBIOS_3_2", "UNIBIOS_3_1",
                         "MVS_ASIA_EUR_V6S1", "MVS_ASIA_EUR_V5S1", "MVS_ASIA_EUR_V3S4",
                         "MVS_USA_V5S2", "MVS_USA_V5S4", "MVS_USA_V5S6", "MVS_JPN_V6",
                         "MVS_JPN_V5", "MVS_JPN_V3S4", "MVS_JPN_J3", "AES_ASIA",
                         "AES_JAPAN", "NEO_MVH_MV1CA", "NEO_MVH_MV1CJ", "DECK_V6", "DEVKIT"},
             0, PEMUConfig::OptId::EMU_NEOBIOS, C2D_CONFIG_RESTART_EMU_NEEDED});
#ifdef __PFBA_ARM__
    // do not use unibios as default on vita for cyclone asm compatibility
    group->getOption(PEMUConfig::OptId::EMU_NEOBIOS)->setArrayIndex(4);
    group->addOption({"FRAMESKIP", {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"},
                      0, PEMUConfig::OptId::EMU_FRAMESKIP, C2D_CONFIG_RESTART_EMU_NEEDED});
#endif

#if defined(__PS4__) || defined(ANDROID)
    // force 48000hz audio output
    get(PEMUConfig::OptId::EMU_AUDIO_FREQ)->setArrayIndex(4);
    get(PEMUConfig::OptId::EMU_AUDIO_FREQ)->setFlags(PEMUConfig::Flags::HIDDEN);
#endif

    // "romlist.cpp" (RomList::build) will also reload config, but we need new roms paths
    PEMUConfig::load();

    // add custom rom path
    PEMUConfig::addRomPath("ARCADE", io->getDataPath() + "arcade/", {HARDWARE_PREFIX_ARCADE, 0, "Arcade"});
#ifndef __PFBN_LIGHT__
    PEMUConfig::addRomPath("CHANNELF", io->getDataPath() + "channelf/", {HARDWARE_PREFIX_CHANNELF, 0, "Channel F"});
    PEMUConfig::addRomPath("COLECO", io->getDataPath() + "coleco/", {HARDWARE_PREFIX_COLECO, 0, "ColecoVision"});
    PEMUConfig::addRomPath("FDS", io->getDataPath() + "fds/", {HARDWARE_PREFIX_FDS, 0, "NES FDS"});
    PEMUConfig::addRomPath("GAMEGEAR", io->getDataPath() + "gamegear/",
                           {HARDWARE_PREFIX_SEGA_GAME_GEAR, 0, "Sega Game Gear"});
    PEMUConfig::addRomPath("MEGADRIVE", io->getDataPath() + "megadrive/",
                           {HARDWARE_PREFIX_SEGA_MEGADRIVE, 0, "Sega Megadrive"});
    PEMUConfig::addRomPath("MSX", io->getDataPath() + "msx/", {HARDWARE_PREFIX_MSX, 0, "MSX"});
    PEMUConfig::addRomPath("NES", io->getDataPath() + "nes/", {HARDWARE_PREFIX_NES, 0, "NES"});
    PEMUConfig::addRomPath("NGP", io->getDataPath() + "ngp/", {HARDWARE_PREFIX_NGP, 0, "Neo Geo Pocket"});
    PEMUConfig::addRomPath("PCE", io->getDataPath() + "pce/", {HARDWARE_PCENGINE_PCENGINE, 0, "PC Engine"});
    PEMUConfig::addRomPath("SG1000", io->getDataPath() + "sg1000/", {HARDWARE_PREFIX_SEGA_SG1000, 0, "Sega SG-1000"});
    PEMUConfig::addRomPath("SGX", io->getDataPath() + "sgx/", {HARDWARE_PCENGINE_SGX, 0, "PC Engine SGX"});
    PEMUConfig::addRomPath("MASTERSYSTEM", io->getDataPath() + "sms/",
                           {HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, 0, "Sega Master System"});
    PEMUConfig::addRomPath("SPECTRUM", io->getDataPath() + "spectrum/", {HARDWARE_PREFIX_SPECTRUM, 0, "ZX Spectrum"});
    PEMUConfig::addRomPath("TG16", io->getDataPath() + "tg16/", {HARDWARE_PCENGINE_TG16, 0, "PC Engine TG16"});
#endif

    // save newly added roms paths
    PEMUConfig::save();

    // create roms paths if needed
    auto paths = getRomPaths();
    for (const auto &path: paths) {
        io->create(path.path);
    }
}

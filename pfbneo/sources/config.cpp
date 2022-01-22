//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include "c2dui.h"
#include "config.h"

using namespace c2d;
using namespace c2dui;

PFBAConfig::PFBAConfig(c2d::Io *io, int version) : Config(io, version) {

    printf("PFBAConfig(%s, v%i)\n", getConfigPath().c_str(), version);

    // add fba default roms paths
    roms_paths.emplace_back(io->getDataPath() + "channelf/");
    roms_paths.emplace_back(io->getDataPath() + "coleco/");
    roms_paths.emplace_back(io->getDataPath() + "fds/");
    roms_paths.emplace_back(io->getDataPath() + "gamegear/");
    roms_paths.emplace_back(io->getDataPath() + "megadrive/");
    roms_paths.emplace_back(io->getDataPath() + "msx/");
    roms_paths.emplace_back(io->getDataPath() + "nes/");
    roms_paths.emplace_back(io->getDataPath() + "ngp/");
    roms_paths.emplace_back(io->getDataPath() + "pce/");
    roms_paths.emplace_back(io->getDataPath() + "sg1000/");
    roms_paths.emplace_back(io->getDataPath() + "sgx/");
    roms_paths.emplace_back(io->getDataPath() + "sms/");
    roms_paths.emplace_back(io->getDataPath() + "spectrum/");
    roms_paths.emplace_back(io->getDataPath() + "tg16/");

    ////////////////////////////////////////////////////////////
    /// pfba custom config
    ////////////////////////////////////////////////////////////

    /// ROMS OPTIONS
    add(Option::Id::ROM_FILTER, "FORCE_60HZ",
        {"OFF", "ON"}, 1, Option::Id::ROM_FORCE_60HZ, Option::Flags::BOOLEAN);
    get(Option::Id::ROM_FORCE_60HZ)->setInfo("You need to restart emulation for this option to take effect...");

    // audio
    add(Option::Id::ROM_FORCE_60HZ, "FORCE_AUDIO_SYNC",
        {"OFF", "ON"}, 0, Option::Id::ROM_AUDIO_SYNC, Option::Flags::BOOLEAN);
    get(Option::Id::ROM_AUDIO_SYNC)->setInfo("You need to restart emulation for this option to take effect...");

    add(Option::Id::ROM_AUDIO_SYNC, "AUDIO_FREQUENCY",
        {"11025", "22050", "32000", "44100", "48000"}, 3, Option::Id::ROM_AUDIO_FREQ, Option::Flags::STRING);
    get(Option::Id::ROM_AUDIO_FREQ)->setInfo("You need to restart emulation for this option to take effect...");

    add(Option::Id::ROM_AUDIO_FREQ, "AUDIO_INTERPOLATION",
        {"0", "1", "3"}, 2, Option::Id::ROM_AUDIO_INTERPOLATION, Option::Flags::STRING);
    get(Option::Id::ROM_AUDIO_INTERPOLATION)->setInfo(
            "You need to restart emulation for this option to take effect...");

    add(Option::Id::ROM_AUDIO_INTERPOLATION, "AUDIO_FM_INTERPOLATION",
        {"0", "1", "3"}, 2, Option::Id::ROM_AUDIO_FMINTERPOLATION, Option::Flags::STRING);
    get(Option::Id::ROM_AUDIO_FMINTERPOLATION)->setInfo(
            "You need to restart emulation for this option to take effect...");

#ifdef __VITA__
    add(Option::Id::ROM_AUDIO_FMINTERPOLATION, "ROTATION",
        {"OFF", "ON", "FLIP", "CAB MODE"}, 1, Option::Id::ROM_ROTATION, Option::Flags::STRING);
#else
    add(Option::Id::ROM_AUDIO_FMINTERPOLATION, "ROTATION",
        {"OFF", "ON", "FLIP"}, 1, Option::Id::ROM_ROTATION, Option::Flags::STRING);
#endif
    add(Option::Id::ROM_ROTATION, "NEOBIOS",
        {"UNIBIOS_4_0", "UNIBIOS_3_3", "UNIBIOS_3_2", "UNIBIOS_3_1",
         "MVS_ASIA_EUR_V6S1", "MVS_ASIA_EUR_V5S1", "MVS_ASIA_EUR_V3S4",
         "MVS_USA_V5S2", "MVS_USA_V5S4", "MVS_USA_V5S6",
         "MVS_JPN_V6", "MVS_JPN_V5", "MVS_JPN_V3S4", "MVS_JPN_J3",
         "AES_ASIA", "AES_JAPAN",
         "NEO_MVH_MV1CA", "NEO_MVH_MV1CJ",
         "DECK_V6", "DEVKIT"},
        0, Option::Id::ROM_NEOBIOS, Option::Flags::STRING);
    get(Option::Id::ROM_NEOBIOS)->setInfo("You need to restart emulation for this option to take effect...");
#ifdef __VITA__
    // do not use unibios as default on vita for cyclone asm compatibility
    get(Option::Id::ROM_NEOBIOS)->setIndex(4);

    add(Option::Id::ROM_NEOBIOS, "FRAMESKIP",
        {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"},
        0, Option::Id::ROM_FRAMESKIP, Option::Flags::STRING);
#endif

#ifdef __PS4__
    // PS4: force 48000hz audio output
    get(Option::Id::ROM_AUDIO_FREQ)->setIndex(4);
    get(Option::Id::ROM_AUDIO_FREQ)->setFlags(Option::Flags::STRING | Option::Flags::HIDDEN);
#endif

    // "c2dui_romlist" will also reload config, but we need new roms paths
    reset();
    load();
    // TODO: fixme
    //save();
}

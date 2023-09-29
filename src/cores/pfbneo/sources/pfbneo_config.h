//
// Created by cpasjuste on 29/05/18.
//

#ifndef PFBN_CONFIG_H
#define PFBN_CONFIG_H

#include "skeleton/pemu_config.h"
#include "pfbneo_utility.h"
#include "burner.h"

class PFBAConfig : public pemu::PEMUConfig {
public:
    PFBAConfig(c2d::Io *io, int version);

    std::vector<GameListInfo> getCoreGameListInfo() override {
        return {
                {{HARDWARE_PREFIX_ARCADE,             0, "Arcade"},             "ARCADE",       "arcade"},
                {{HARDWARE_PREFIX_CHANNELF,           0, "Channel F"},          "CHANNELF",     "channelf"},
                {{HARDWARE_PREFIX_COLECO,             0, "ColecoVision"},       "COLECO",       "coleco"},
                {{HARDWARE_PREFIX_FDS,                0, "NES FDS"},            "FDS",          "fds"},
                {{HARDWARE_PREFIX_SEGA_GAME_GEAR,     0, "Sega Game Gear"},     "GAMEGEAR",     "gamegear"},
                {{HARDWARE_PREFIX_SEGA_MEGADRIVE,     0, "Sega Megadrive"},     "MEGADRIVE",    "megadrive"},
                {{HARDWARE_PREFIX_MSX,                0, "MSX"},                "MSX",          "msx"},
                {{HARDWARE_PREFIX_NES,                0, "NES"},                "NES",          "nes"},
                {{HARDWARE_PREFIX_NGP,                0, "Neo Geo Pocket"},      "NGP",          "ngp"},
                {{HARDWARE_PCENGINE_PCENGINE,         0, "PC Engine"},          "PCE",          "pce"},
                {{HARDWARE_PREFIX_SEGA_SG1000,        0, "Sega SG-1000"},       "SG1000",       "sg1000"},
                {{HARDWARE_PCENGINE_SGX,              0, "PC Engine SGX"},      "SGX",          "sgx"},
                {{HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, 0, "Sega Master System"}, "MASTERSYSTEM", "sms"},
                {{HARDWARE_PREFIX_SPECTRUM,           0, "ZX Spectrum"},        "SPECTRUM",     "spectrum"},
                {{HARDWARE_PCENGINE_TG16,             0, "PC Engine TG16"},     "TG16",         "tg16"}
        };
    }

    std::vector<int> getCoreHiddenOptionToEnable() override {
        return {
                PEMUConfig::OptId::UI_FILTER_CLONES,
                PEMUConfig::OptId::UI_FILTER_SYSTEM
        };
    }

    std::string getCoreVersion() override {
        return "fbneo: " + std::string(szAppBurnVer);
    }

    std::vector<std::string> getCoreSupportedExt() override {
        return {".zip"};
    }
};

#endif //PFBN_CONFIG_H

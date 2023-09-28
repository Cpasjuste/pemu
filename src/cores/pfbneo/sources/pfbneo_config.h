//
// Created by cpasjuste on 29/05/18.
//

#ifndef PFBN_CONFIG_H
#define PFBN_CONFIG_H

#include "skeleton/pemu_config.h"
#include "burner.h"

class PFBAConfig : public pemu::PEMUConfig {
public:
    PFBAConfig(c2d::Io *io, int version);

    std::vector<GameListInfo> getCoreGameListInfo() override {
        return {
                {{75,                  0,  "Arcade"},                      "ARCADE",       "arcade",    "gamelist.xml"},
                {{SYSTEM_ID_CHANNELF,  0,  "Channel F"},                   "CHANNELF",     "channelf",  "gamelist_channelf.xml"},
                {{SYSTEM_ID_COLECO,    0,  "Colecovision"},                "COLECO",       "coleco",    "gamelist_coleco.xml"},
                {{SYSTEM_ID_NES_FDS,   3,  "Family Computer Disk System"}, "FDS",          "fds",       "gamelist_fds.xml"},
                {{SYSTEM_ID_GAMEGEAR,  0,  "Game Gear"},                   "GAMEGEAR",     "gamegear",  "gamelist_gamegear.xml"},
                {{SYSTEM_ID_MEGADRIVE, 0,  "Megadrive"},                   "MEGADRIVE",    "megadrive", "gamelist_megadrive.xml"},
                {{SYSTEM_ID_MSX,       0,  "MSX"},                         "MSX",          "msx",       "gamelist_msx.xml"},
                {{SYSTEM_ID_NES,       0,  "NES"},                         "NES",          "nes",       "gamelist_nes.xml"},
                {{SYSTEM_ID_NGP,       0,  "Neo-Geo Pocket"},              "NGP",          "ngp",       "gamelist_ngp.xml"},
                {{SYSTEM_ID_PCE,       0,  "PC Engine"},                   "PCE",          "pce",       "gamelist_pce.xml"},
                {{SYSTEM_ID_SG1000,    2,  "SG-1000"},                     "SG1000",       "sg1000",    "gamelist_sg1000.xml"},
                {{SYSTEM_ID_SGX,       31, "PC Engine SuperGrafx"},        "SGX",          "sgx",       "gamelist_sgx.xml"},
                {{SYSTEM_ID_SMS,       0,  "Master System"},               "MASTERSYSTEM", "sms",       "gamelist_sms.xml"},
                {{SYSTEM_ID_ZX3,       0,  "ZX Spectrum"},                 "SPECTRUM",     "spectrum",  "gamelist_spectrum.xml"},
                {{SYSTEM_ID_TG16,      31, "PC Engine TurboGrafx"},        "TG16",         "tg16",      "gamelist_tg16.xml"}
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

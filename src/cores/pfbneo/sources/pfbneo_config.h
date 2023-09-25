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
                {{75,  0,  "Arcade"},                      "ARCADE",       "arcade",    "gamelist.xml"},
                {{80,  0,  "Channel F"},                   "CHANNELF",     "channelf",  "gamelist_channelf.xml"},
                {{48,  0,  "Colecovision"},                "COLECO",       "coleco",    "gamelist_coleco.xml"},
                {{106, 3,  "Family Computer Disk System"}, "FDS",          "fds",       "gamelist_fds.xml"},
                {{21,  0,  "Game Gear"},                   "GAMEGEAR",     "gamegear",  "gamelist_gamegear.xml"},
                {{1,   0,  "Megadrive"},                   "MEGADRIVE",    "megadrive", "gamelist_megadrive.xml"},
                {{113, 0,  "MSX"},                         "MSX",          "msx",       "gamelist_msx.xml"},
                {{3,   0,  "NES"},                         "NES",          "nes",       "gamelist_nes.xml"},
                {{82,  0,  "Neo-Geo Pocket Color"},        "NGP",          "ngp",       "gamelist_ngp.xml"},
                {{31,  0,  "PC Engine"},                   "PCE",          "pce",       "gamelist_pce.xml"},
                {{109, 2,  "SG-1000"},                     "SG1000",       "sg1000",    "gamelist_sg1000.xml"},
                {{105, 31, "PC Engine SuperGrafx"},        "SGX",          "sgx",       "gamelist_sgx.xml"},
                {{2,   0,  "Master System"},               "MASTERSYSTEM", "sms",       "gamelist_sms.xml"},
                {{76,  0,  "ZX Spectrum"},                 "SPECTRUM",     "spectrum",  "gamelist_spectrum.xml"},
                {{758, 31, "PC Engine TurboGrafx"},        "TG16",         "tg16",      "gamelist_tg16.xml"}
        };
    }

    std::vector<int> getCoreHiddenOptionToEnable() override {
        return {
                PEMUConfig::Id::GUI_FILTER_CLONES,
                PEMUConfig::Id::GUI_FILTER_SYSTEM,
                PEMUConfig::Id::GUI_FILTER_ROTATION,
                PEMUConfig::Id::GUI_FILTER_RESOLUTION
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

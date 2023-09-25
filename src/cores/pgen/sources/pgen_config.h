//
// Created by cpasjuste on 29/05/18.
//

#ifndef PGEN_CONFIG_H
#define PGEN_CONFIG_H

#include "skeleton/pemu_config.h"
#include "osd.h"

class PGENConfig : public pemu::PEMUConfig {
public:
    PGENConfig(c2d::Io *io, int version);

    ~PGENConfig() override {
        printf("PGENConfig::~PGENConfig()\n");
    }

    std::vector<GameListInfo> getCoreGameListInfo() override {
        return {
                {{1,   0, "Megadrive"},     "MEGADRIVE",    "megadrive",    "gamelist.xml"},
                {{2,   0, "Master System"}, "MASTERSYSTEM", "mastersystem", "gamelist_sms.xml"},
                {{21,  0, "Game Gear"},     "GAMEGEAR",     "gamegear",     "gamelist_gamegear.xml"},
                {{20,  0, "Mega-CD"},       "MEGACD",       "megacd",       "gamelist_megacd.xml"},
                {{109, 2, "SG-1000"},       "SG1000",       "sg1000",       "gamelist_sg1000.xml"}
        };
    }

    std::vector<int> getCoreHiddenOptionToEnable() override {
        return {PEMUConfig::Id::GUI_FILTER_SYSTEM};
    }

    std::string getCoreVersion() override {
        return VERSION;
    }

    std::vector<std::string> getCoreSupportedExt() override {
        return {".zip", ".md", ".smd", ".gen", ".bin",
                ".mdx", ".sms", ".gg", ".sg", ".68k"};
    }
};

#endif //PGEN_CONFIG_H

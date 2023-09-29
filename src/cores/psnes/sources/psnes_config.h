//
// Created by cpasjuste on 29/05/18.
//

#ifndef PSNES_CONFIG_H
#define PSNES_CONFIG_H

#include "skeleton/pemu_config.h"
#include "snes9x.h"

class PSNESConfig : public pemu::PEMUConfig {

public:
    PSNESConfig(c2d::Io *io, int version);

    std::vector<GameListInfo> getCoreGameListInfo() override {
        return {
                {{4, 0, "Super Nintendo"}, "SNES", "roms"}
        };
    }

    std::string getCoreVersion() override {
        return "snes9x: " + std::string(VERSION);
    }

    std::vector<std::string> getCoreSupportedExt() override {
        return {".zip", ".sfc", ".smc", ".swc", ".fig"};
    }
};

#endif //PSNES_CONFIG_H

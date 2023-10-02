//
// Created by cpasjuste on 29/05/18.
//

#ifndef PGBA_CONFIG_H
#define PGBA_CONFIG_H

#include "skeleton/pemu_config.h"
extern "C" {
#include "mgba/core/version.h"
}

class PGBAConfig : public pemu::PEMUConfig {
public:
    PGBAConfig(c2d::Io *io, int version);

    std::string getCoreVersion() override {
        return std::string(projectName) + projectVersion;
    }

    std::vector<std::string> getCoreSupportedExt() override {
        return {".zip", ".gba", ".bin"};
    }
};

#endif //PGBA_CONFIG_H

//
// Created by cpasjuste on 29/05/18.
//

#ifndef PNES_CONFIG_H
#define PNES_CONFIG_H

#include "skeleton/pemu_config.h"

class PNESConfig : public pemu::PEMUConfig {
public:
    PNESConfig(c2d::Io *io, int version);

    std::string getCoreVersion() override;

    std::vector<std::string> getCoreSupportedExt() override;
};

#endif //PNES_CONFIG_H

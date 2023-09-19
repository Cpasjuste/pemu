//
// Created by cpasjuste on 29/05/18.
//

#ifndef PFBA_CONFIG_H
#define PFBA_CONFIG_H

#include "skeleton/pemu_config.h"

class PFBAConfig : public pemu::PEMUConfig {
public:
    PFBAConfig(c2d::Io *io, int version);

    std::string getCoreVersion() override;
};

#endif //PFBA_CONFIG_H

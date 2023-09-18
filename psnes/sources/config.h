//
// Created by cpasjuste on 29/05/18.
//

#ifndef PSNES_CONFIG_H
#define PSNES_CONFIG_H

#include "c2dui_config_new.h"

class PSNESConfig : public c2dui::ConfigNew {

public:
    PSNESConfig(c2d::Io *io, int version);

};

#endif //PSNES_CONFIG_H

//
// Created by cpasjuste on 29/05/18.
//

#ifndef PNES_CONFIG_H
#define PNES_CONFIG_H

#include "c2dui_config.h"

class PNESConfig : public c2dui::ConfigNew {

public:
    PNESConfig(c2d::Io *io, int version);

};

#endif //PNES_CONFIG_H

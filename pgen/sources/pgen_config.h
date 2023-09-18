//
// Created by cpasjuste on 29/05/18.
//

#ifndef PGEN_CONFIG_H
#define PGEN_CONFIG_H

#include "c2dui_config.h"

class PGENConfig : public c2dui::ConfigNew {
public:
    PGENConfig(c2d::Io *io, int version);
};

#endif //PGEN_CONFIG_H

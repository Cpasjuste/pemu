//
// Created by cpasjuste on 29/05/18.
//

#ifndef PNES_CONFIG_H
#define PNES_CONFIG_H

#include "c2dui_config.h"

class PNESConfig : public c2dui::Config {

public:

    PNESConfig(c2d::Renderer *renderer, const std::string &home, int version);

};

#endif //PNES_CONFIG_H

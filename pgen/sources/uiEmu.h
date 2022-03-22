//
// Created by cpasjuste on 01/06/18.
//

#ifndef PGEN_UIEMU_H
#define PGEN_UIEMU_H

#include <string>

class PGENUiEmu : public c2dui::UiEmu {

public:

    explicit PGENUiEmu(c2dui::UiMain *ui);

    int load(const ss_api::Game &game) override;

    void stop() override;

    bool onInput(c2d::Input::Player *players) override;

    void onUpdate() override;
};

#endif //PGEN_UIEMU_H

//
// Created by cpasjuste on 01/06/18.
//

#ifndef PNES_UIEMU_H
#define PNES_UIEMU_H

#include <string>

class PNESGuiEmu : public c2dui::UiEmu {

public:

    explicit PNESGuiEmu(c2dui::UiMain *ui);

    int load(const ss_api::Game &game) override;

    void stop() override;

    void nestopia_config_init();

    int nestopia_core_init(const char *rom_path);

    bool onInput(c2d::Input::Player *players) override;

    void onUpdate() override;
};

#endif //PNES_UIEMU_H

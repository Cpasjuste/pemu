//
// Created by cpasjuste on 01/06/18.
//

#ifndef PGEN_UI_EMU_H
#define PGEN_UI_EMU_H

#include <string>

class PGENUiEmu : public c2dui::UiEmu {

public:

    explicit PGENUiEmu(c2dui::UiMain *ui);

    int load(const ss_api::Game &game) override;

    void stop() override;

    bool onInput(c2d::Input::Player *players) override;

    void onUpdate() override;

private:
    void loadBios();
};

#endif //PGEN_UI_EMU_H

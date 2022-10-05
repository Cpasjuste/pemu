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

    void onUpdate() override;

    void resizeVideo(bool isGameGear = false);

private:
    void loadBios();

    void loadBram();

    void saveBram();

    void loadSram();

    void saveSram();
};

#endif //PGEN_UI_EMU_H

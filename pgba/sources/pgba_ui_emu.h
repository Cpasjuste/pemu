//
// Created by cpasjuste on 01/06/18.
//

#ifndef PGBA_UI_EMU_H
#define PGBA_UI_EMU_H

#include <string>

class PGBAUiEmu : public c2dui::UiEmu {

public:

    explicit PGBAUiEmu(c2dui::UiMain *ui);

    int load(const ss_api::Game &game) override;

    void stop() override;

    bool onInput(c2d::Input::Player *players) override;

    void onUpdate() override;

    void resizeVideo(bool isGameGear = false);

private:
    void loadBios();

    void loadBram();

    void saveBram();

    void loadSram();

    void saveSram();
};

#endif //PGBA_UI_EMU_H

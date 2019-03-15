//
// Created by cpasjuste on 01/06/18.
//

#ifndef PSNES_UIEMU_H
#define PSNES_UIEMU_H

#include <string>

class PSNESUIEmu : public c2dui::UIEmu {

public:

    PSNESUIEmu(c2dui::UIMain *ui);

    int load(c2dui::RomList::Rom *rom);

    void stop();

    bool onInput(c2d::Input::Player *players) override;

    void onDraw(c2d::Transform &transform, bool draw = true) override;
};

#endif //PSNES_UIEMU_H

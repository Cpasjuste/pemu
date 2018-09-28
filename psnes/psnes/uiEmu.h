//
// Created by cpasjuste on 01/06/18.
//

#ifndef PSNES_UIEMU_H
#define PSNES_UIEMU_H

#include <string>

class PSNESGuiEmu : public c2dui::UIEmu {

public:

    PSNESGuiEmu(c2dui::UIMain *ui);

    int run(c2dui::RomList::Rom *rom);

    void stop();

    int update();
};

#endif //PSNES_UIEMU_H

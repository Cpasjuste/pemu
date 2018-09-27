//
// Created by cpasjuste on 01/06/18.
//

#ifndef PNES_UIEMU_H
#define PNES_UIEMU_H

#include <string>

class PNESGuiEmu : public c2dui::C2DUIGuiEmu {

public:

    explicit PNESGuiEmu(c2dui::C2DUIGuiMain *ui);

    int run(c2dui::C2DUIRomList::Rom *rom);

    void stop();

    int update();

    void nestopia_config_init();

    int nestopia_core_init(const char *rom_path);

private:

};

#endif //PNES_UIEMU_H

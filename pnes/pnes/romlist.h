//
// Created by cpasjuste on 29/05/18.
//

#ifndef PNES_ROMLIST_H
#define PNES_ROMLIST_H

#include <string>

class PNESRomList : public c2dui::C2DUIRomList {

public:

    PNESRomList(c2dui::C2DUIGuiMain *ui, const std::string &emuVersion);

    ~PNESRomList();

    void build() override;

    bool isHardware(int hardware, int type) override {
        return true;
    }
};

#endif //PNES_ROMLIST_H

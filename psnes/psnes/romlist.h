//
// Created by cpasjuste on 29/05/18.
//

#ifndef PSNES_ROMLIST_H
#define PSNES_ROMLIST_H

#include <string>

class PSNESRomList : public c2dui::RomList {

public:

    PSNESRomList(c2dui::UIMain *ui, const std::string &emuVersion);

    ~PSNESRomList();

    void buildNoDb(bool use_icons);

    void build() override;

    bool isHardware(int hardware, int type) override {
        return true;
    }
};

#endif //PSNES_ROMLIST_H

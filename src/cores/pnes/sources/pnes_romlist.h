//
// Created by cpasjuste on 02/04/2020.
//

#ifndef PNES_ROM_LIST_H
#define PNES_ROM_LIST_H

class PNESRomList : public pemu::RomList {
public:
    PNESRomList(pemu::UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters)
            : pemu::RomList(ui, emuVersion, filters) {};

    void build(const ss_api::System &system = {}) override {
        RomList::build({3, 0, "NES"});
    };
};

#endif //PNES_ROM_LIST_H

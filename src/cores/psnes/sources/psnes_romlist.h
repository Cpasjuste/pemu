//
// Created by cpasjuste on 02/04/2020.
//

#ifndef PSNES_ROM_LIST_H
#define PSNES_ROM_LIST_H

class PSNESRomList : public pemu::RomList {
public:
    PSNESRomList(pemu::UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters)
            : pemu::RomList(ui, emuVersion, filters) {};

    void build(const ss_api::System &system = {}) override {
        RomList::build({4, 0, "Super Nintendo"});
    };
};

#endif //PSNES_ROM_LIST_H

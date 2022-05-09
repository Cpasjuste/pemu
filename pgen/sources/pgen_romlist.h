//
// Created by cpasjuste on 02/04/2020.
//

#ifndef PGEN_ROMLIST_H
#define PGEN_ROMLIST_H

class PGENRomList : public c2dui::RomList {

public:

    PGENRomList(c2dui::UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters)
            : c2dui::RomList(ui, emuVersion, filters) {};

    void build(bool addArcadeSystem = false, const ss_api::System &system = {}) override;
};

#endif //PGEN_ROMLIST_H

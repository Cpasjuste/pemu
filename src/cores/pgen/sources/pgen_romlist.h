//
// Created by cpasjuste on 02/04/2020.
//

#ifndef PGEN_ROMLIST_H
#define PGEN_ROMLIST_H

class PGENRomList : public pemu::RomList {

public:

    PGENRomList(pemu::UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters)
            : pemu::RomList(ui, emuVersion, filters) {};

    void build(const ss_api::System &system = {}) override;
};

#endif //PGEN_ROMLIST_H

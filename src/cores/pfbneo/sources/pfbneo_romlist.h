//
// Created by cpasjuste on 02/04/2020.
//

#ifndef PFBA_ROMLIST_H
#define PFBA_ROMLIST_H

class PFBARomList : public pemu::RomList {

public:

    PFBARomList(pemu::UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters)
            : pemu::RomList(ui, emuVersion, filters) {};

    void build(const ss_api::System &system = {}) override;
};

#endif //PFBA_ROMLIST_H

//
// Created by cpasjuste on 28/09/23.
//

#ifndef PEMU_PFBNEO_ROMLIST_H
#define PEMU_PFBNEO_ROMLIST_H

class PFBNRomList : public RomList {
public:
    PFBNRomList(UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters)
            : RomList(ui, emuVersion, filters) {};

    void build(const ss_api::GameList::GameAddedCb &cb = nullptr) override;
};

#endif //PEMU_PFBNEO_ROMLIST_H

//
// Created by cpasjuste on 28/09/23.
//

#ifndef PEMU_PFBNEO_ROMLIST_H
#define PEMU_PFBNEO_ROMLIST_H

class PFBNRomList : public RomList {
public:
    struct FBNEOGameInfo {
        std::string name;
        std::string parent;
        std::string system;
        int systemId;
    };

    PFBNRomList(UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters)
            : RomList(ui, emuVersion, filters) {};

    void build(const ss_api::System &system = {}) override;

    static FBNEOGameInfo getGameInfo(const std::string &driver);
};

#endif //PEMU_PFBNEO_ROMLIST_H

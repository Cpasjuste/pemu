//
// Created by cpasjuste on 02/04/2020.
//

#ifndef PFBA_ROMLIST_H
#define PFBA_ROMLIST_H

class PFBARomList : public c2dui::RomList {

public:

    PFBARomList(c2dui::UiMain *ui, const std::string &emuVersion);

    void build() override;
};

#endif //PFBA_ROMLIST_H

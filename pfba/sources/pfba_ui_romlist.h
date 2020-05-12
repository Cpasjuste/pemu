//
// Created by cpasjuste on 14/01/18.
//

#ifndef PFBA_UI_ROMLIST_CLASSIC_H
#define PFBA_UI_ROMLIST_CLASSIC_H

namespace c2dui {

    class PFBAUIRomList : public c2dui::UIRomList {

    public:

        PFBAUIRomList(UIMain *ui, RomList *romList, const c2d::Vector2f &size) :
                UIRomList(ui, romList, size) {}

        void filterRomList() override;

        void sortRomList() override;
    };
}

#endif //C2DUI_UI_ROMLIST_CLASSIC_H

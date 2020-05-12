//
// Created by cpasjuste on 14/01/18.
//

#ifndef PFBA_UI_ROMLIST_CLASSIC_H
#define PFBA_UI_ROMLIST_CLASSIC_H

#include "c2dui_ui_romlist.h"
#include "c2dui_romlist.h"

namespace c2dui {

    class PFBAUIRomListClassic : public c2dui::UIRomListClassic {

    public:
        void filterRomList() override;

        void sortRomList() override;
    };
}

#endif //C2DUI_UI_ROMLIST_CLASSIC_H

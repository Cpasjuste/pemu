//
// Created by cpasjuste on 14/01/18.
//

#ifndef C2DUI_UI_ROMLIST_CLASSIC_H
#define C2DUI_UI_ROMLIST_CLASSIC_H

#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "c2dui_listbox.h"

class UIRomInfo;

namespace c2dui {

    class UIRomListClassic : public c2dui::UIRomList {

    public:

        UIRomListClassic(UIMain *ui, RomList *romList, const c2d::Vector2f &size);

        ~UIRomListClassic() override;

        bool onInput(c2d::Input::Player *players) override;

        void onUpdate() override;

        void updateRomList() override;

        ss_api::Game getSelection() override;

    private:

        UIRomInfo *rom_info = nullptr;
        UIListBox *list_box = nullptr;
        int rom_index = 0;
        bool show_preview = false;
        int title_loaded = 0;
        int load_delay = 500;
        c2d::C2DClock timer_load;
    };
}

#endif //C2DUI_UI_ROMLIST_CLASSIC_H

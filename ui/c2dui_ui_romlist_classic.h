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

        void setVisibility(c2d::Visibility visibility, bool tweenPlay = false) override;

        void updateRomList() override;

        ss_api::Game getSelection() override;

    private:

        UIRomInfo *romInfo = nullptr;
        UIListBox *listBox = nullptr;

        c2d::C2DClock timer_load_info;
        int timer_load_info_delay = 300;
        int timer_load_info_done = 0;

        c2d::C2DClock timer_load_video;
        int timer_load_video_done = 0;
    };
}

#endif //C2DUI_UI_ROMLIST_CLASSIC_H

//
// Created by cpasjuste on 14/01/18.
//

#ifndef C2DUI_UI_ROMLIST_H
#define C2DUI_UI_ROMLIST_H

#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "cross2d/widgets/listbox.h"

namespace c2dui {

    class UIRomList : public c2d::RectangleShape {

    public:

        UIRomList(UIMain *ui, RomList *romList, const c2d::Vector2f &size);

        ~UIRomList() override;

        virtual void filterRomList();

        virtual void updateRomList();

        virtual RomList *getRomList();

        virtual ss_api::Game getSelection();

        virtual c2d::Texture *getPreviewTexture(const ss_api::Game &game);

        virtual void setVideoSnapDelay(int delay);

    protected:

        UIMain *ui = nullptr;
        RomList *romList = nullptr;
        ss_api::GameList gameList;
        int timer_load_video_delay = 5000;
    };
}

#endif //C2DUI_UI_ROMLIST_H

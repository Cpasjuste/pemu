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

    protected:

        UIMain *ui = nullptr;
        RomList *rom_list = nullptr;
        ss_api::GameList gameList;
    };
}

#endif //C2DUI_UI_ROMLIST_H

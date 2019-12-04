//
// Created by cpasjuste on 19/10/16.
//

#ifndef C2DUI_ROMLIST_H
#define C2DUI_ROMLIST_H

#include <vector>
#include "ss_gamelist.h"

#define HARDWARE_PREFIX_ALL 0xffffffff

namespace c2dui {

    class UIMain;

    class RomList {

    public:

        RomList(UIMain *ui, const std::string &emuVersion);

        virtual ~RomList();

        virtual void build();

        void addFav(ss_api::Game *rom);

        void removeFav(ss_api::Game *rom);

        enum RomState {
            NOT_WORKING = 0,
            MISSING,
            WORKING
        };

        UIMain *ui;
        c2d::RectangleShape *rect;
        c2d::Text *text;
        ss_api::GameList gameList;
        std::vector<std::string> paths;
        char icon_path[1024];
        char text_str[512];
        float time_start = 0;
    };
}

#endif //C2DUI_ROMLIST_H

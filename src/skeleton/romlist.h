//
// Created by cpasjuste on 19/10/16.
//

#ifndef C2DUI_ROMLIST_H
#define C2DUI_ROMLIST_H

#include <vector>

namespace pemu {

    class UiMain;

    class RomList {

    public:

        RomList(UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters);

        virtual ~RomList();

        virtual void build(bool addArcadeSystem = false, const ss_api::System &system = {});

        void addFav(const ss_api::Game &game);

        void removeFav(const ss_api::Game &game);

        void setLoadingText(const char *format, ...);

        UiMain *ui;
        c2d::RectangleShape *rect;
        c2d::Text *text;
        ss_api::GameList *gameList = nullptr;
        ss_api::GameList *gameListFav = nullptr;
        //std::vector<std::string> paths;
        //char icon_path[1024];
        float time_start = 0;

    protected:
        std::vector<std::string> filters;
    };
}

#endif //C2DUI_ROMLIST_H

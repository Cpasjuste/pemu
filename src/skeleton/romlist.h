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

        virtual void build(const ss_api::GameList::GameAddedCb &cb = nullptr);

        virtual void initFav();

        void addFav(const ss_api::Game &game);

        void removeFav(const ss_api::Game &game);

        void setLoadingText(const char *format, ...);

        UiMain *ui;
        c2d::RectangleShape *rect;
        c2d::Text *text;
        ss_api::GameList *gameList = nullptr;
        ss_api::GameList *gameListFav = nullptr;
        ss_api::GameList::GameAddedCb p_cb = nullptr;

    protected:
        std::vector<std::string> filters;
        size_t m_games_count = 0;
        size_t m_games_available_count = 0;
    };
}

#endif //C2DUI_ROMLIST_H

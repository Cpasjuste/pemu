//
// Created by cpasjuste on 19/10/16.
//

#ifndef C2DUI_ROMLIST_H
#define C2DUI_ROMLIST_H

#include <vector>
#include "pemu_config.h"

namespace pemu {
    class UiMain;

    class RomList {
    public:
        enum PreviewType {
            Tex,
            Vid
        };

        RomList(UiMain *ui, const std::string &emuVersion, const std::vector<std::string> &filters);

        RomList(PEMUConfig *cfg, const std::string &emuVersion, const std::vector<std::string> &filters);

        virtual ~RomList();

        virtual void build(const ss_api::GameList::GameAddedCb &cb = nullptr);

        virtual void initFav();

        void addFav(const ss_api::Game &game);

        void removeFav(const ss_api::Game &game);

        void setLoadingText(const char *format, ...);

        UiMain *ui = nullptr;
        c2d::Text *text = nullptr;
        c2d::RectangleShape *rect = nullptr;
        ss_api::GameList *gameList = nullptr;
        ss_api::GameList *gameListFav = nullptr;
        ss_api::GameList::GameAddedCb p_cb = nullptr;

    protected:
        std::vector<std::string> filters;
        size_t m_count = 0;
        PEMUConfig *mConfig = nullptr;
    };
}

#endif //C2DUI_ROMLIST_H

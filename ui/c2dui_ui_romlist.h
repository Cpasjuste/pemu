//
// Created by cpasjuste on 14/01/18.
//

#ifndef C2DUI_UI_ROMLIST_H
#define C2DUI_UI_ROMLIST_H

class UIRomInfo;

namespace c2dui {

    class UIRomList : public c2d::RectangleShape {

    public:

        UIRomList(UiMain *ui, RomList *romList, const c2d::Vector2f &size);

        ~UIRomList() override;

        virtual void filterRomList();

        virtual void sortRomList();

        virtual void updateRomList();

        virtual ss_api::Game getSelection();

        virtual RomList *getRomList();

        virtual RectangleShape *getBlur() { return blur; };

        virtual c2d::Texture *getPreviewTexture(const ss_api::Game &game);

        virtual std::string getPreviewVideo(const ss_api::Game &game);

        virtual void setVideoSnapDelay(int delay);

        void setVisibility(c2d::Visibility visibility, bool tweenPlay = false) override;

    protected:

        bool onInput(c2d::Input::Player *players) override;

        void onUpdate() override;

        UiMain *ui = nullptr;
        RomList *romList = nullptr;
        ss_api::GameList gameList;
        UIRomInfo *romInfo = nullptr;
        UIListBox *listBox = nullptr;
        RectangleShape *blur = nullptr;
        c2d::Text *titleText = nullptr;

        c2d::C2DClock timer_load_info;
        int timer_load_info_delay = 300;
        int timer_load_info_done = 0;

        c2d::C2DClock timer_load_video;
        int timer_load_video_delay = 5000;
        int timer_load_video_done = 0;
    };
}

#endif //C2DUI_UI_ROMLIST_H

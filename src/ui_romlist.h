//
// Created by cpasjuste on 14/01/18.
//

#ifndef C2DUI_UI_ROMLIST_H
#define C2DUI_UI_ROMLIST_H

class UIRomInfo;

namespace pemu {

    class UIRomList : public SkinnedRectangle {

    public:
        enum PreviewType {
            Tex,
            Vid
        };

        UIRomList(UiMain *ui, RomList *romList, const c2d::Vector2f &size);

        ~UIRomList() override;

        virtual void filterRomList();

        virtual void sortRomList();

        virtual void updateRomList();

        virtual ss_api::Game getSelection();

        virtual RomList *getRomList();

        virtual RectangleShape *getBlur() { return pBlur; };

        virtual std::string getPreview(const ss_api::Game &game, PreviewType type);

        virtual void setVideoSnapDelay(int delay);

        void setVisibility(c2d::Visibility visibility, bool tweenPlay = false) override;

    protected:

        bool onInput(c2d::Input::Player *players) override;

        void onUpdate() override;

        UiMain *pMain = nullptr;
        RomList *pRomList = nullptr;
        UIRomInfo *pRomInfo = nullptr;
        UIListBox *pListBox = nullptr;
        RectangleShape *pBlur = nullptr;
        SkinnedText *pTitleText = nullptr;
        ss_api::GameList mGameList;

        c2d::C2DClock mTimerLoadInfo;
#ifdef __3DS__
        int mTimerLoadInfoDelay = 1000;
#else
        int mTimerLoadInfoDelay = 300;
#endif
        int mTimerLoadInfoDone = 0;
        c2d::C2DClock mTimerLoadVideo;
        int mTimerLoadVideoDelay = 5000;
        int mTimerLoadVideoDone = 0;
    };
}

#endif //C2DUI_UI_ROMLIST_H

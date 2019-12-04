//
// Created by cpasjuste on 16/02/18.
//

#ifndef C2DUI_MENU_STATE_H
#define C2DUI_MENU_STATE_H

class UIStateList;

namespace c2dui {

    class UIStateMenu : public c2d::RectangleShape {

    public:

        explicit UIStateMenu(UIMain *ui);

        void setVisibility(c2d::Visibility visibility, bool tweenPlay = false) override;

        bool onInput(c2d::Input::Player *players) override;

        virtual bool loadStateCore(const char *path) { return false; };

        virtual bool saveStateCore(const char *path) { return false; };

    private:

        UIMain *ui = nullptr;
        UIStateList *uiStateList = nullptr;
        c2d::Text *title = nullptr;
        bool isEmuRunning = false;
    };
}

#endif //C2DUI_MENU_STATE_H

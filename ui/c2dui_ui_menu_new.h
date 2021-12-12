//
// Created by cpasjuste on 30/01/18.
//

#ifndef C2DUI_UI_MENU_NEW_H
#define C2DUI_UI_MENU_NEW_H

class MenuLine;

namespace c2dui {

    class UIMenuNew : public c2d::RectangleShape {

    public:

        explicit UIMenuNew(UIMain *uiMain);

        ~UIMenuNew() override;

        bool onInput(c2d::Input::Player *players) override;

        void load(bool isRomMenu = false);

        UIMain *getUi() { return ui; };

        bool isRom() const { return isRomMenu; };

        virtual bool isOptionHidden(Option *option) { return false; };

        void setVisibility(c2d::Visibility visibility, bool tweenPlay = false) override;

    private:

        void updateLines();

        UIMain *ui = nullptr;
        c2d::Text *title = nullptr;
        c2d::RectangleShape *highlight = nullptr;
        std::vector<MenuLine *> lines;
        float alpha = 230;

        std::vector<Option> options;
        c2d::TweenPosition *tweenPosition;
        Skin::TextGroup textGroup;

        float lineHeight = 0;
        int maxLines = 0;
        int optionIndex = 0;
        int highlightIndex = 0;

        bool isRomMenu = false;
        bool isEmuRunning = false;
    };
}

#endif //C2DUI_UI_MENU_NEW_H

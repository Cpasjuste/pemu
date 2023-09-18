//
// Created by cpasjuste on 14/01/18.
//

#ifndef GUI_H
#define GUI_H

#define INPUT_DELAY 200

namespace c2dui {

    class UiMain : public c2d::C2DRenderer {

    public:
        explicit UiMain(c2d::Io *io, const c2d::Vector2f &size = c2d::Vector2f(0, 0));

        void init(UIRomList *romList, UiMenu *menu, UiEmu *emu, UiStateMenu *state);

        void setConfig(ConfigNew *cfg);

        void setSkin(Skin *skin);

        void onUpdate() override;

        void updateInputMapping(bool isRomCfg);

        Skin *getSkin();

        ConfigNew *getConfig();

        UIHighlight *getUiHighlight();

        UIRomList *getUiRomList();

        UiEmu *getUiEmu();

        UiMenu *getUiMenu();

        UiStateMenu *getUiStateMenu();

        UiStatusBox *getUiStatusBox();

        UIProgressBox *getUiProgressBox();

        c2d::MessageBox *getUiMessageBox();

        int getFontSize();

        c2d::Vector2f getScaling();

        bool done = false;

    private:
        ConfigNew *pConfig = nullptr;
        Skin *pSkin = nullptr;
        UIHighlight *pHighlight = nullptr;
        UiMenu *pMenu = nullptr;
        UIRomList *pRomList = nullptr;
        UiEmu *pEmu = nullptr;
        UiStateMenu *pState = nullptr;
        UIProgressBox *pProgressBox = nullptr;
        c2d::MessageBox *pMessageBox = nullptr;
        UiStatusBox *pStatusBox = nullptr;
        c2d::C2DClock mTimer;
        unsigned int mOldKeys = 0;
    };
}

#endif //GUI_H

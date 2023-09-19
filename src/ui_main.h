//
// Created by cpasjuste on 14/01/18.
//

#ifndef GUI_H
#define GUI_H

#define INPUT_DELAY 200

namespace pemu {

    class UiMain : public c2d::C2DRenderer {

    public:
        explicit UiMain(c2d::Io *io, const c2d::Vector2f &size = c2d::Vector2f(0, 0));

        void init(UIRomList *romList, UiMenu *menu, UiEmu *emu, UiMenuState *state);

        void setConfig(PEMUConfig *cfg);

        void setSkin(Skin *skin);

        void onUpdate() override;

        void updateInputMapping(bool isRomCfg);

        Skin *getSkin();

        PEMUConfig *getConfig();

        UIHighlight *getUiHighlight();

        UIRomList *getUiRomList();

        UiEmu *getUiEmu();

        UiMenu *getUiMenu();

        UiMenuState *getUiStateMenu();

        UiStatusBox *getUiStatusBox();

        UIProgressBox *getUiProgressBox();

        c2d::MessageBox *getUiMessageBox();

        int getFontSize();

        c2d::Vector2f getScaling();

        bool done = false;

    private:
        PEMUConfig *pConfig = nullptr;
        Skin *pSkin = nullptr;
        UIHighlight *pHighlight = nullptr;
        UiMenu *pMenu = nullptr;
        UIRomList *pRomList = nullptr;
        UiEmu *pEmu = nullptr;
        UiMenuState *pState = nullptr;
        UIProgressBox *pProgressBox = nullptr;
        c2d::MessageBox *pMessageBox = nullptr;
        UiStatusBox *pStatusBox = nullptr;
        c2d::C2DClock mTimer;
        unsigned int mOldKeys = 0;
    };
}

#endif //GUI_H

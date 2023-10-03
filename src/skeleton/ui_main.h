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

        void onUpdate() override;

        void updateInputMapping(bool isRomCfg);

        Skin *getSkin() { return pSkin; }

        void setSkin(Skin *s) { pSkin = s; }

        PEMUConfig *getConfig() { return pConfig; }

        UIHighlight *getUiHighlight() { return pHighlight; }

        UIRomList *getUiRomList() { return pRomList; }

        UiEmu *getUiEmu() { return pEmu; }

        UiMenu *getUiMenu() { return pMenu; }

        UiMenuState *getUiStateMenu() { return pState; }

        UiStatusBox *getUiStatusBox() { return pStatusBox; }

        UIProgressBox *getUiProgressBox() { return pProgressBox; }

        c2d::MessageBox *getUiMessageBox() { return pMessageBox; }

        int getFontSize() { return (int) ((float) C2D_DEFAULT_CHAR_SIZE * pSkin->getScaling().y); }

        c2d::Vector2f getScaling() { return pSkin->getScaling(); }

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

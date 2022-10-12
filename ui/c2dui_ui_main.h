//
// Created by cpasjuste on 14/01/18.
//

#ifndef GUI_H
#define GUI_H

#define INPUT_DELAY 200

namespace c2dui {

    class UiMain : public c2d::C2DRenderer {

    public:
        explicit UiMain(const c2d::Vector2f &size, c2d::Io *io, Config *cfg);

        void init(UIRomList *uiRomList, UiMenu *uiMenu,
                  UiEmu *uiEmu, UiStateMenu *uiState);

        void setConfig(Config *cfg);

        void setSkin(Skin *skin);

        void onUpdate() override;

        void updateInputMapping(bool isRomCfg);

        Skin *getSkin();

        Config *getConfig();

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
        Config *config = nullptr;
        Skin *skin = nullptr;
        UIHighlight *uiHighlight = nullptr;
        UiMenu *uiMenu = nullptr;
        UIRomList *uiRomList = nullptr;
        UiEmu *uiEmu = nullptr;
        UiStateMenu *uiState = nullptr;
        UIProgressBox *uiProgressBox = nullptr;
        c2d::MessageBox *uiMessageBox = nullptr;
        UiStatusBox *uiStatusBox = nullptr;
        c2d::C2DClock timer;
        unsigned int oldKeys = 0;
    };
}

#endif //GUI_H

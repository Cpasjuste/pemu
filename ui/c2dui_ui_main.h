//
// Created by cpasjuste on 14/01/18.
//

#ifndef GUI_H
#define GUI_H

#define UI_KEY_FILTER_ROMS      100
#define UI_KEY_SHOW_ROMLIST     101
#define UI_KEY_SHOW_MEMU_UI     102
#define UI_KEY_SHOW_MEMU_ROM    103
#define UI_KEY_SHOW_MEMU_STATE  104
#define UI_KEY_RUN_ROM          105
#define UI_KEY_RESUME_ROM       106
#define UI_KEY_STOP_ROM         107

#define UI_MARGIN   16
#define INPUT_DELAY 200

#define COL_GREEN   Color(153, 255, 51)
#define COL_YELLOW  Color(255, 225, 51)
#define COL_ORANGE  Color(255, 153, 51)
#define COL_RED     Color(255, 51, 51)

namespace c2dui {

    class UiMain : public c2d::C2DRenderer {

    public:

        explicit UiMain(const c2d::Vector2f &size, c2d::Io *io, Config *cfg);

        ~UiMain() override;

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

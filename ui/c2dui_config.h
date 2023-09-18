//
// Created by cpasjuste on 13/09/23.
//

#ifndef C2DUI_CONFIG_NEW_H
#define C2DUI_CONFIG_NEW_H

#define C2D_CONFIG_RESTART_NEEDED "YOU NEED TO RESTART THE APPLICATION AFTER CHANGING THIS OPTION"

namespace c2dui {
    class ConfigNew : public c2d::config::Config {
    public:
        enum Flags {
            INPUT = BIT(1),
            HIDDEN = BIT(2)
        };

        enum Id {
            MENU_MAIN = 0,
            GUI_SHOW_FAVORITES,
            GUI_SHOW_AVAILABLE,
            GUI_SHOW_ZIP_NAMES,
            GUI_FILTER_CLONES,
            GUI_FILTER_SYSTEM,
            GUI_FILTER_EDITOR,
            GUI_FILTER_DEVELOPER,
            GUI_FILTER_PLAYERS,
            GUI_FILTER_RATING,
            GUI_FILTER_ROTATION,
            GUI_FILTER_RESOLUTION,
            GUI_FILTER_DATE,
            GUI_FILTER_GENRE,
            GUI_SHOW_ICONS,
            GUI_SCREEN_WIDTH,
            GUI_SCREEN_HEIGHT,
            GUI_FULLSCREEN,
            GUI_SKIN,
            GUI_SKIN_ASPECT,
            GUI_FONT_SCALING,
            GUI_VIDEO_SNAP_DELAY,
            MENU_ROM_OPTIONS,
            ROM_SCALING,
            ROM_SCALING_MODE,
            ROM_FILTER,
            ROM_SHADER,
#ifdef __VITA__
            ROM_WAIT_RENDERING,
#endif
            ROM_FORCE_60HZ,
            ROM_AUDIO_SYNC,
            ROM_AUDIO_FREQ,
            ROM_AUDIO_INTERPOLATION,
            ROM_AUDIO_FMINTERPOLATION,
            ROM_ROTATION,
            ROM_SHOW_FPS,
            ROM_FRAMESKIP,
            ROM_NEOBIOS,
            ROM_PSNES_CHEATS,
            ROM_PSNES_BLOCK_VRAM,
            ROM_PSNES_TRANSPARENCY,
            ROM_PSNES_DISPLAY_MESSAGES,
            ROM_PSNES_FRAMESKIP,
            ROM_PSNES_TURBO_MODE,
            ROM_PSNES_TURBO_FRAMESKIP,
            MENU_JOYPAD,
            JOY_UP,
            JOY_DOWN,
            JOY_LEFT,
            JOY_RIGHT,
            JOY_A,
            JOY_B,
            JOY_X,
            JOY_Y,
            JOY_LT,
            JOY_RT,
            JOY_LB,
            JOY_RB,
            JOY_SELECT,
            JOY_START,
            JOY_MENU1,
            JOY_MENU2,
            JOY_AXIS_LX,
            JOY_AXIS_LY,
            JOY_AXIS_RX,
            JOY_AXIS_RY,
            JOY_DEADZONE,
#ifndef NO_KEYBOARD
            MENU_KEYBOARD,
            KEY_UP,
            KEY_DOWN,
            KEY_LEFT,
            KEY_RIGHT,
            KEY_A,
            KEY_B,
            KEY_X,
            KEY_Y,
            KEY_LT,
            KEY_RT,
            KEY_LB,
            KEY_RB,
            KEY_SELECT,
            KEY_START,
            KEY_MENU1,
            KEY_MENU2,
#endif
            END
        };

        ConfigNew(c2d::Io *io, const std::string &name, int version = 1);

        ~ConfigNew();

        bool loadGame(const ss_api::Game &game);

        bool save(bool isGame = false);

        c2d::config::Option *get(int id, bool isGame = false);

        c2d::config::Option *getOption(int id, bool isGame = false);

        std::string getRomPath(const std::string &name = "");

        std::vector<c2d::Input::ButtonMapping> getKeyboardMapping(int player, bool isGame = false);

        std::vector<c2d::Input::ButtonMapping> getJoystickMapping(int player, bool isGame = false);

        c2d::Vector2i getJoystickAxisLeftMapping(int player, bool isGame = false);

        c2d::Vector2i getJoystickAxisRightMapping(int player, bool isGame = false);

        int getJoystickDeadZone(int player, bool isGame = false);

    private:
        c2d::config::Config *p_game_config = nullptr;
        c2d::Io *p_io = nullptr;
    };
}

#define CFG_ID_ROMS (ConfigNew::Id::END + 1)

#endif //C2DUI_CONFIG_NEW_H

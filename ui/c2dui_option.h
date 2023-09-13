//
// Created by cpasjuste on 05/12/16.
//

#ifndef _C2DUI_OPTION_H_
#define _C2DUI_OPTION_H_

#include <string>
#include <vector>

#define BIT(n) (1U<<(n))

namespace c2dui {

    class Option {

    public:

        enum Flags {
            MENU = BIT(0),
            INTEGER = BIT(1),
            STRING = BIT(2),
            BOOLEAN = BIT(3),
            INPUT = BIT(4),
            HIDDEN = BIT(5)
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
        };

        Option() = default;

        Option(const std::string &name, const std::vector<std::string> &options,
               int defaultValueIndex, int id, unsigned int flags = INTEGER);

        std::string getName() const;

        std::string getInfo() const;

        void setInfo(const std::string &info);

        std::string getValueString() const;

        void setValueString(const std::string &value);

        int getValueInt(int defValue = 0);

        void setValueInt(int value);

        bool getValueBool();

        void setValueBool(bool value);

        std::vector<std::string> *getValues();

        int getIndex();

        void setIndex(int index);

        int getId();

        void setId(int id);

        unsigned int getFlags();

        void setFlags(unsigned int flags);

        void next();

        void prev();

        void set(const Option &option);

        int size();

    private:
        std::string name;
        std::string info;
        std::vector<std::string> options;
        std::string current_option;
        unsigned int flags = INTEGER;
        int id = 0;
    };
}

#endif //_C2DUI_OPTION_H_

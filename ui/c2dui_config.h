//
// Created by cpasjuste on 05/12/16.
//

#ifndef _C2DUI_CONFIG_H_
#define _C2DUI_CONFIG_H_

#include "libconfig.h"

namespace c2dui {

    class Config {

    public:

        Config(c2d::Io *io, int version);

        virtual ~Config() = default;

        void load(const ss_api::Game &game = ss_api::Game());

        void save(const ss_api::Game &game = ss_api::Game());

        void reset();

        std::string getHomePath();

        std::string getConfigPath();

        std::string getRomPath(int n = 0);

        std::vector<std::string> getRomPaths();

        std::vector<Option> *get(bool isRom = false);

        Option *get(int id, bool isRom = false);

        bool add(int target,
                         const std::string &text, const std::vector<std::string> &values,
                         int defaultValue, int id, unsigned int flags);

        void append(const std::string &text, const std::vector<std::string> &values,
                            int defaultValue, int id, unsigned int flags);

        void append(const std::string &text, int value, int id, unsigned int flags);

        bool hide(int id, bool isRom = false);

        virtual int *getPlayerInputKeys(int player, bool isRom = false);

        virtual int *getPlayerInputButtons(int player, bool isRom = false);

    protected:
        std::vector<std::string> roms_paths;

    private:
        std::vector<Option> options_gui;
        std::vector<Option> options_rom;
        std::string configPath;
        std::string dataPath;

        int version;
        int keyboard_keys[KEY_COUNT];
        int joystick_keys[KEY_COUNT];
    };
}

#endif //_C2DUI_CONFIG_H_

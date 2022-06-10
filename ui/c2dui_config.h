//
// Created by cpasjuste on 05/12/16.
//

#ifndef _C2DUI_CONFIG_H_
#define _C2DUI_CONFIG_H_

#include "libconfig.h"

#define FBN_PATH_ARCADE     0
#define FBN_PATH_CHANNELF   1
#define FBN_PATH_COLECO     2
#define FBN_PATH_FDS        3
#define FBN_PATH_GAMEGEAR   4
#define FBN_PATH_MEGADRIVE  5
#define FBN_PATH_MSX        6
#define FBN_PATH_NES        7
#define FBN_PATH_NGP        8
#define FBN_PATH_PCE        9
#define FBN_PATH_SG1000     10
#define FBN_PATH_SGX        11
#define FBN_PATH_SMS        12
#define FBN_PATH_SPECTRUM   13
#define FBN_PATH_TG16       14

namespace c2dui {

    class Config {

    public:

        Config(c2d::Io *io, int version, const std::string &defaultRomsPath = "roms/");

        virtual ~Config() = default;

        void load(const ss_api::Game &game = ss_api::Game());

        void save(const ss_api::Game &game = ss_api::Game());

        void reset();

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

        virtual std::vector<c2d::Input::ButtonMapping> getKeyboardMapping(int player, bool isRom = false);

        virtual std::vector<c2d::Input::ButtonMapping> getJoystickMapping(int player, bool isRom = false);

        virtual c2d::Vector2i getJoystickAxisLeftMapping(int player, bool isRom = false);

        virtual c2d::Vector2i getJoystickAxisRightMapping(int player, bool isRom = false);

        virtual int getJoystickDeadZone(int player, bool isRom = false);

        c2d::Vector2f getScreenSize();

    protected:
        std::vector<std::string> roms_paths;

    private:
        c2d::Io *m_io;
        std::vector<Option> options_gui;
        std::vector<Option> options_rom;
        std::string configPath;
        std::string dataPath;

        int version;
    };
}

#endif //_C2DUI_CONFIG_H_

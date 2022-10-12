//
// Created by cpasjuste on 05/12/16.
//

#include "c2dui.h"

Config::Config(UiMain *ui, int ver, const std::string &defaultRomsPath) {
    m_ui = ui;
    dataPath = m_ui->getIo()->getDataPath();
    configPath = dataPath + "config.cfg";
    version = ver;

    printf("Config(%s, v%i)\n", configPath.c_str(), ver);

    /// custom screens size
    int joyDeadZoneIndex = 3;
    std::string device = C2DDevice::getName();
    if (device == "pocket2") {
        joyDeadZoneIndex = 11;
    }

    /// add default roms paths
    roms_paths.clear();
    roms_paths.emplace_back(m_ui->getIo()->getDataPath() + defaultRomsPath);

    /// default options available for all cores
    /////////////////////////////////////////////////
    /// main/gui config
    /////////////////////////////////////////////////
    append("MAIN", {"MAIN"}, 0, Option::Id::MENU_MAIN, Option::Flags::MENU);
    append("SHOW", {"ALL", "AVAILABLE", "FAVORITES"}, 0, Option::Id::GUI_SHOW_ALL,
           Option::Flags::STRING);
    append("SHOW_CLONES", {"OFF", "ON"}, 0,
           Option::Id::GUI_FILTER_CLONES, Option::Flags::BOOLEAN | Option::Flags::HIDDEN);
    append("SHOW_ZIP_NAMES", {"OFF", "ON"}, 1, Option::Id::GUI_SHOW_ZIP_NAMES,
           Option::Flags::BOOLEAN);
    append("SHOW_ICONS", {"OFF", "ON"}, 0, Option::Id::GUI_SHOW_ICONS,
           Option::Flags::BOOLEAN | Option::Flags::HIDDEN);
    get()->at(get()->size() - 1).setInfo(
            "YOU NEED TO RESTART THE APPLICATION AFTER CHANGING THIS OPTION");
#if 0 // TODO
    append("SCREEN_WIDTH", (int) C2DDevice::getResolution().x, Option::Id::GUI_SCREEN_WIDTH,
           Option::Flags::INTEGER | Option::Flags::HIDDEN);
    append("SCREEN_HEIGHT", (int) C2DDevice::getResolution().y, Option::Id::GUI_SCREEN_HEIGHT,
           Option::Flags::INTEGER | Option::Flags::HIDDEN);
#endif
#ifdef __FULLSCREEN__
    append("FULLSCREEN", {"OFF", "ON"}, 0, Option::Id::GUI_FULLSCREEN, Option::Flags::BOOLEAN);
    get()->at(get()->size() - 1).setInfo("YOU NEED TO RESTART THE APPLICATION AFTER CHANGING THIS OPTION");
#endif

    // build  skin list
    std::vector<std::string> skins;
    // add default skins from romfs
    skins.emplace_back("default");
    // add skins from romfs dir
    auto files = m_ui->getIo()->getDirList(m_ui->getIo()->getRomFsPath() + "skins/", true);
    for (auto &file: files) {
        if (file.type != c2d::Io::Type::Directory || file.name[0] == '.') {
            continue;
        }
        // only append skin name if it
        if (std::find(skins.begin(), skins.end(), file.name) == skins.end()) {
            skins.emplace_back(file.name);
            printf("skin found: %s\n", file.path.c_str());
        }
    }
    // add skins from data dir
    files = m_ui->getIo()->getDirList(dataPath + "skins/", true);
    for (auto &file: files) {
        if (file.type != c2d::Io::Type::Directory || file.name[0] == '.') {
            continue;
        }
        // only append skin name if it
        if (std::find(skins.begin(), skins.end(), file.name) == skins.end()) {
            skins.emplace_back(file.name);
            printf("skin found: %s\n", file.path.c_str());
        }
    }
    // set "default" skin index (or "big" skin on screen height < 240)
    if (!get(Option::Id::GUI_SKIN)) {
        int index = 0;
        for (size_t i = 0; i < skins.size(); i++) {
            if (m_ui->getSize().y > 240) {
                if (skins.at(i) == "default") {
                    index = (int) i;
                }
            } else {
                if (skins.at(i) == "big") {
                    index = (int) i;
                }
            }
        }
        append("SKIN", skins, index, Option::Id::GUI_SKIN, Option::Flags::STRING);
    }
    get()->at(get()->size() - 1).setInfo(
            "YOU NEED TO RESTART THE APPLICATION AFTER CHANGING THIS OPTION");

    append("VIDEO_SNAP_DELAY", {"3", "5", "7", "10"}, 1,
           Option::Id::GUI_VIDEO_SNAP_DELAY, Option::Flags::STRING);

    /////////////////////////////////////////////////
    /// default rom config
    /////////////////////////////////////////////////
    append("EMULATION", {"EMULATION"}, 0, Option::Id::MENU_ROM_OPTIONS, Option::Flags::MENU);
    if (m_ui->getSize().y > 1080) {
        append("SCALING", {"NONE", "2X", "3X", "4X", "5", "6", "7", "8", "9", "FIT", "FULL"},
               6, Option::Id::ROM_SCALING, Option::Flags::STRING);
    } else if (m_ui->getSize().y > 720) {
        append("SCALING", {"NONE", "2X", "3X", "4X", "FIT", "FULL"},
               4, Option::Id::ROM_SCALING, Option::Flags::STRING);
    } else if (m_ui->getSize().y > 544) {
        append("SCALING", {"NONE", "2X", "3X", "FIT", "FULL"},
               3, Option::Id::ROM_SCALING, Option::Flags::STRING);
    } else if (m_ui->getSize().y > 240) {
        append("SCALING", {"NONE", "2X", "FIT", "FULL"},
               2, Option::Id::ROM_SCALING, Option::Flags::STRING);
    } else {
        append("SCALING", {"NONE", "FIT", "FULL"},
               1, Option::Id::ROM_SCALING, Option::Flags::STRING);
    }
    append("SCALING_MODE", {"AUTO", "ASPECT", "INTEGER"}, 1,
           Option::Id::ROM_SCALING_MODE, Option::Flags::STRING);
    append("FILTER", {"POINT", "LINEAR"}, 0, Option::Id::ROM_FILTER, Option::Flags::STRING);
#ifdef __VITA__
    append("WAIT_RENDERING", {"OFF", "ON"}, 1, Option::Id::ROM_WAIT_RENDERING, Option::Flags::BOOLEAN);
#endif
    append("SHOW_FPS", {"OFF", "ON"}, 0, Option::Id::ROM_SHOW_FPS, Option::Flags::BOOLEAN);

    /// joysticks config
    append("JOYPAD", {"JOYPAD"}, 0, Option::Id::MENU_JOYPAD, Option::Flags::MENU);
    append("JOY_UP", KEY_JOY_UP_DEFAULT, Option::Id::JOY_UP, Option::Flags::INPUT);
    append("JOY_DOWN", KEY_JOY_DOWN_DEFAULT, Option::Id::JOY_DOWN, Option::Flags::INPUT);
    append("JOY_LEFT", KEY_JOY_LEFT_DEFAULT, Option::Id::JOY_LEFT, Option::Flags::INPUT);
    append("JOY_RIGHT", KEY_JOY_RIGHT_DEFAULT, Option::Id::JOY_RIGHT, Option::Flags::INPUT);
    append("JOY_A", KEY_JOY_A_DEFAULT, Option::Id::JOY_A, Option::Flags::INPUT);
    append("JOY_B", KEY_JOY_B_DEFAULT, Option::Id::JOY_B, Option::Flags::INPUT);
    append("JOY_X", KEY_JOY_X_DEFAULT, Option::Id::JOY_X, Option::Flags::INPUT);
    append("JOY_Y", KEY_JOY_Y_DEFAULT, Option::Id::JOY_Y, Option::Flags::INPUT);
    append("JOY_LT", KEY_JOY_LT_DEFAULT, Option::Id::JOY_LT, Option::Flags::INPUT);
    append("JOY_RT", KEY_JOY_RT_DEFAULT, Option::Id::JOY_RT, Option::Flags::INPUT);
    append("JOY_LB", KEY_JOY_LB_DEFAULT, Option::Id::JOY_LB, Option::Flags::INPUT);
    append("JOY_RB", KEY_JOY_RB_DEFAULT, Option::Id::JOY_RB, Option::Flags::INPUT);
    append("JOY_SELECT", KEY_JOY_SELECT_DEFAULT, Option::Id::JOY_SELECT, Option::Flags::INPUT);
    append("JOY_START", KEY_JOY_START_DEFAULT, Option::Id::JOY_START, Option::Flags::INPUT);
    append("JOY_MENU1", KEY_JOY_MENU1_DEFAULT, Option::Id::JOY_MENU1, Option::Flags::INPUT);
    append("JOY_MENU2", KEY_JOY_MENU2_DEFAULT, Option::Id::JOY_MENU2, Option::Flags::INPUT);
    // TODO: add gui option for axis in option menu
    append("JOY_AXIS_LX", KEY_JOY_AXIS_LX, Option::Id::JOY_AXIS_LX,
           Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_AXIS_LY", KEY_JOY_AXIS_LY, Option::Id::JOY_AXIS_LY,
           Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_AXIS_RX", KEY_JOY_AXIS_RX, Option::Id::JOY_AXIS_RX,
           Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_AXIS_RY", KEY_JOY_AXIS_RY, Option::Id::JOY_AXIS_RY,
           Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_DEADZONE",
           {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
            "18000", "20000", "22000", "24000", "26000", "28000", "30000"}, joyDeadZoneIndex,
           Option::Id::JOY_DEADZONE, Option::Flags::STRING);
#ifndef NO_KEYBOARD
    // keyboard
    append("KEYBOARD", {"KEYBOARD"}, 0, Option::Id::MENU_KEYBOARD, Option::Flags::MENU);
    append("KEY_UP", KEY_KB_UP_DEFAULT, Option::Id::KEY_UP, Option::Flags::INPUT);
    append("KEY_DOWN", KEY_KB_DOWN_DEFAULT, Option::Id::KEY_DOWN, Option::Flags::INPUT);
    append("KEY_LEFT", KEY_KB_LEFT_DEFAULT, Option::Id::KEY_LEFT, Option::Flags::INPUT);
    append("KEY_RIGHT", KEY_KB_RIGHT_DEFAULT, Option::Id::KEY_RIGHT, Option::Flags::INPUT);
    append("KEY_A", KEY_KB_A_DEFAULT, Option::Id::KEY_A, Option::Flags::INPUT);
    append("KEY_B", KEY_KB_B_DEFAULT, Option::Id::KEY_B, Option::Flags::INPUT);
    append("KEY_X", KEY_KB_X_DEFAULT, Option::Id::KEY_X, Option::Flags::INPUT);
    append("KEY_Y", KEY_KB_Y_DEFAULT, Option::Id::KEY_Y, Option::Flags::INPUT);
    append("KEY_LT", KEY_KB_LT_DEFAULT, Option::Id::KEY_LT, Option::Flags::INPUT);
    append("KEY_RT", KEY_KB_RT_DEFAULT, Option::Id::KEY_RT, Option::Flags::INPUT);
    append("KEY_LB", KEY_KB_LB_DEFAULT, Option::Id::KEY_LB, Option::Flags::INPUT);
    append("KEY_RB", KEY_KB_RB_DEFAULT, Option::Id::KEY_RB, Option::Flags::INPUT);
    append("KEY_SELECT", KEY_KB_SELECT_DEFAULT, Option::Id::KEY_SELECT, Option::Flags::INPUT);
    append("KEY_START", KEY_KB_START_DEFAULT, Option::Id::KEY_START, Option::Flags::INPUT);
    append("KEY_MENU1", KEY_KB_MENU1_DEFAULT, Option::Id::KEY_MENU1, Option::Flags::INPUT);
    append("KEY_MENU2", KEY_KB_MENU2_DEFAULT, Option::Id::KEY_MENU2, Option::Flags::INPUT);
#endif
}

void Config::load(const ss_api::Game &game) {
    config_t cfg;
    config_init(&cfg);

    bool isRomCfg = game.id > 0;
    std::vector<Option> *options = isRomCfg ? &options_rom : &options_gui;
    std::string path = configPath;
    if (isRomCfg) {
        path = dataPath;
        path += "configs/";
        path += Utility::removeExt(game.path);
        path += ".cfg";
    }

    printf("Config::load: %s ...", path.c_str());

    if (config_read_file(&cfg, path.c_str()) == CONFIG_TRUE) {

        config_setting_t *settings_root = config_lookup(&cfg, "CONFIG");
        if (settings_root != nullptr) {
            printf("OK\n");
#ifdef __UPDATE_CONFIG__
            // verify cfg version
            int cfg_version = 0;
            if ((config_setting_lookup_int(settings_root, "VERSION", &cfg_version) == 0)
                || cfg_version != version) {
                // update cfg to newer version
                printf("CONFIG VERSION (%i) != EXE VERSION (%i)\n", cfg_version, version);
                save(game);
                config_destroy(&cfg);
                return;
            }
#endif
            config_setting_t *settings = nullptr;
            if (!isRomCfg) {
                settings = config_setting_lookup(settings_root, "ROMS_PATHS");
                if (settings != nullptr) {
                    char p[MAX_PATH];
                    for (size_t i = 0; i < roms_paths.size(); i++) {
                        memset(p, 0, MAX_PATH);
                        snprintf(p, MAX_PATH, "ROMS_PATH%i", (int) i);
                        const char *value = nullptr;
                        if (config_setting_lookup_string(settings, p, &value) != 0) {
                            if (m_ui->getIo()->exist(value)) {
                                roms_paths[i] = value;
                                if (!roms_paths[i].empty() && roms_paths[i].back() != '/') {
                                    roms_paths[i] += '/';
                                }
                            }
                            printf("%s: %s\n", p, roms_paths[i].c_str());
                        }
                    }
                }
            }

            for (auto &option: *options) {
                unsigned int flags = option.getFlags();
                if (flags & Option::Flags::MENU) {
                    settings = config_setting_lookup(settings_root, option.getName().c_str());
                }
                if (settings != nullptr) {
                    if (flags & Option::Flags::INTEGER || flags & Option::Flags::INPUT) {
                        int value = 0;
                        if (config_setting_lookup_int(settings, option.getName().c_str(), &value) !=
                            0) {
                            option.setValueInt(value);
                            //printf("Config::load: OPTION: %s, VALUE: %i\n", option.getName().c_str(),
                            //       option.getValueInt());
                        }
                    } else {
                        const char *value;
                        if (config_setting_lookup_string(settings, option.getName().c_str(),
                                                         &value) != 0) {
                            option.setValueString(value);
                            //printf("Config::load: OPTION: %s, VALUE: %s\n", option.getName().c_str(),
                            //       option.getValueString().c_str());
                        }
                    }
                }
            }
        } else {
            printf("NOK: root configuration not found\n");
            save(game);
            config_destroy(&cfg);
            return;
        }
    } else {
        printf("NOK: file not found\n");
        // reset default rom options for other roms usage
        reset();
        // no need to save default rom config
        if (!isRomCfg) {
            save(ss_api::Game());
        }
    }

    config_destroy(&cfg);
}

void Config::save(const ss_api::Game &game) {
    config_t cfg{};
    config_init(&cfg);

    bool isRomCfg = game.id > 0;
    std::vector<Option> *options = isRomCfg ? &options_rom : &options_gui;
    std::string path = configPath;
    if (isRomCfg) {
        path = dataPath;
        path += "configs/";
        path += Utility::removeExt(game.path);
        path += ".cfg";
    }

    printf("Config::save: %s\n", path.c_str());

    // create root
    config_setting_t *setting_root = config_root_setting(&cfg);

    // create main group
    config_setting_t *setting_fba = config_setting_add(setting_root, "CONFIG", CONFIG_TYPE_GROUP);
    // add version
    config_setting_t *setting_cfg = config_setting_add(setting_fba, "VERSION", CONFIG_TYPE_INT);
    config_setting_set_int(setting_cfg, version);

    config_setting_t *sub_setting = nullptr;

    if (!isRomCfg) {
        sub_setting = config_setting_add(setting_fba, "ROMS_PATHS", CONFIG_TYPE_GROUP);
        for (unsigned int i = 0; i < roms_paths.size(); i++) {
            char p[MAX_PATH];
            snprintf(p, MAX_PATH, "ROMS_PATH%i", i);
            config_setting_t *setting = config_setting_add(sub_setting, p, CONFIG_TYPE_STRING);
            config_setting_set_string(setting, roms_paths[i].c_str());
        }
    }

    for (auto &option: *options) {
        unsigned int flags = option.getFlags();
        if (flags & Option::Flags::MENU) {
            sub_setting = config_setting_add(setting_fba, option.getName().c_str(),
                                             CONFIG_TYPE_GROUP);
            continue;
        }

        if (flags & Option::Flags::INTEGER || flags & Option::Flags::INPUT) {
            config_setting_t *setting = config_setting_add(sub_setting, option.getName().c_str(),
                                                           CONFIG_TYPE_INT);
            config_setting_set_int(setting, option.getValueInt());
        } else {
            config_setting_t *setting = config_setting_add(sub_setting, option.getName().c_str(),
                                                           CONFIG_TYPE_STRING);
            config_setting_set_string(setting, option.getValueString().c_str());
        }
    }

    config_write_file(&cfg, path.c_str());
    config_destroy(&cfg);
}

void Config::reset() {
    options_rom.clear();

    size_t start = 0, end = options_gui.size();
    for (size_t i = 0; i < options_gui.size(); i++) {
        if (options_gui[i].getId() == Option::Id::MENU_ROM_OPTIONS) {
            start = i;
            break;
        }
    }

    for (size_t i = start; i < end; i++) {
        options_rom.emplace_back(options_gui[i]);
    }
}

std::string Config::getConfigPath() {
    return configPath;
}

std::string Config::getRomPath(int n) {
    if ((size_t) n >= roms_paths.size()) {
        return roms_paths.at(0);
    } else {
        return roms_paths.at(n);
    }
}

std::vector<std::string> Config::getRomPaths() {
    return roms_paths;
}

std::vector<Option> *Config::get(bool isRom) {
    return isRom ? &options_rom : &options_gui;
}

Option *Config::get(int index, bool isRom) {
    std::vector<Option> *options = get(isRom);

    for (auto &option: *options) {
        if (option.getId() == index) {
            return &option;
        }
    }
    return nullptr;
}

bool Config::add(int target, const std::string &text, const std::vector<std::string> &values,
                 int defaultValue, int index, unsigned int flags) {
    for (unsigned int i = 0; i < options_gui.size(); i++) {
        if (options_gui[i].getId() == target) {
            options_gui.insert(options_gui.begin() + i + 1,
                               Option(text, values, defaultValue, index, flags));
            return true;
        }
    }

    return false;
}

void Config::append(const std::string &text, const std::vector<std::string> &values,
                    int defaultValue, int id, unsigned int flags) {
    options_gui.emplace_back(text, values, defaultValue, id, flags);
}

void Config::append(const std::string &text, int value, int id, unsigned int flags) {
    append(text, {std::to_string(value)}, 0, id, flags);
}

bool Config::hide(int index, bool isRom) {
    std::vector<Option> *options = get(isRom);

    for (auto &option: *options) {
        if (option.getId() == index) {
            unsigned int flags = option.getFlags();
            flags |= Option::HIDDEN;
            option.setFlags(flags);
            return true;
        }
    }

    return false;
}

std::vector<c2d::Input::ButtonMapping> Config::getKeyboardMapping(int player, bool isRom) {
#ifndef NO_KEYBOARD
    return {
            {Input::Button::Up,     get(Option::Id::KEY_UP, isRom)->getValueInt()},
            {Input::Button::Down,   get(Option::Id::KEY_DOWN, isRom)->getValueInt()},
            {Input::Button::Left,   get(Option::Id::KEY_LEFT, isRom)->getValueInt()},
            {Input::Button::Right,  get(Option::Id::KEY_RIGHT, isRom)->getValueInt()},
            {Input::Button::Select, get(Option::Id::KEY_SELECT, isRom)->getValueInt()},
            {Input::Button::Start,  get(Option::Id::KEY_START, isRom)->getValueInt()},
            {Input::Button::A,      get(Option::Id::KEY_A, isRom)->getValueInt()},
            {Input::Button::B,      get(Option::Id::KEY_B, isRom)->getValueInt()},
            {Input::Button::X,      get(Option::Id::KEY_X, isRom)->getValueInt()},
            {Input::Button::Y,      get(Option::Id::KEY_Y, isRom)->getValueInt()},
            {Input::Button::LT,     get(Option::Id::KEY_LT, isRom)->getValueInt()},
            {Input::Button::RT,     get(Option::Id::KEY_RT, isRom)->getValueInt()},
            {Input::Button::LB,     get(Option::Id::KEY_LB, isRom)->getValueInt()},
            {Input::Button::RB,     get(Option::Id::KEY_RB, isRom)->getValueInt()},
            //{Input::Button::LS,     get(Option::Id::KEY_UP, isRom)->getValueInt()},
            //{Input::Button::RS,     get(Option::Id::KEY_UP, isRom)->getValueInt()},
            {Input::Button::Menu1,  get(Option::Id::KEY_MENU1, isRom)->getValueInt()},
            {Input::Button::Menu2,  get(Option::Id::KEY_MENU2, isRom)->getValueInt()}
    };
#else
    return {};
#endif
}

std::vector<c2d::Input::ButtonMapping> Config::getJoystickMapping(int player, bool isRom) {
    // TODO: allow per player config ?
    return {
            {Input::Button::Up,     get(Option::Id::JOY_UP, isRom)->getValueInt()},
            {Input::Button::Down,   get(Option::Id::JOY_DOWN, isRom)->getValueInt()},
            {Input::Button::Left,   get(Option::Id::JOY_LEFT, isRom)->getValueInt()},
            {Input::Button::Right,  get(Option::Id::JOY_RIGHT, isRom)->getValueInt()},
            {Input::Button::Select, get(Option::Id::JOY_SELECT, isRom)->getValueInt()},
            {Input::Button::Start,  get(Option::Id::JOY_START, isRom)->getValueInt()},
            {Input::Button::A,      get(Option::Id::JOY_A, isRom)->getValueInt()},
            {Input::Button::B,      get(Option::Id::JOY_B, isRom)->getValueInt()},
            {Input::Button::X,      get(Option::Id::JOY_X, isRom)->getValueInt()},
            {Input::Button::Y,      get(Option::Id::JOY_Y, isRom)->getValueInt()},
            {Input::Button::LT,     get(Option::Id::JOY_LT, isRom)->getValueInt()},
            {Input::Button::RT,     get(Option::Id::JOY_RT, isRom)->getValueInt()},
            {Input::Button::LB,     get(Option::Id::JOY_LB, isRom)->getValueInt()},
            {Input::Button::RB,     get(Option::Id::JOY_RB, isRom)->getValueInt()},
            //{Input::Button::LS,     get(Option::Id::KEY_UP, isRom)->getValueInt()},
            //{Input::Button::RS,     get(Option::Id::KEY_UP, isRom)->getValueInt()},
            {Input::Button::Menu1,  get(Option::Id::JOY_MENU1, isRom)->getValueInt()},
            {Input::Button::Menu2,  get(Option::Id::JOY_MENU2, isRom)->getValueInt()}
    };
}

c2d::Vector2i Config::getJoystickAxisLeftMapping(int player, bool isRom) {
    return {
            get(Option::Id::JOY_AXIS_LX, isRom)->getValueInt(),
            get(Option::Id::JOY_AXIS_LY, isRom)->getValueInt()
    };
}

c2d::Vector2i Config::getJoystickAxisRightMapping(int player, bool isRom) {
    return {
            get(Option::Id::JOY_AXIS_RX, isRom)->getValueInt(),
            get(Option::Id::JOY_AXIS_RY, isRom)->getValueInt()
    };
}

int Config::getJoystickDeadZone(int player, bool isRom) {
    return get(Option::Id::JOY_DEADZONE, isRom)->getValueInt();
}

// TODO
/*
c2d::Vector2f Config::getScreenSize() {
    Option *opt = get(Option::Id::GUI_FULLSCREEN);
    if (opt && opt->getValueBool()) {
        return {0, 0};
    } else {
        return {
                (float) get(Option::Id::GUI_SCREEN_WIDTH)->getValueInt(),
                (float) get(Option::Id::GUI_SCREEN_HEIGHT)->getValueInt()
        };
    }
}
*/

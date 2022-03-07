//
// Created by cpasjuste on 05/12/16.
//

#include "c2dui.h"
#include "c2dui_config.h"

Config::Config(c2d::Io *io, int ver) {

    dataPath = io->getDataPath();
    configPath = dataPath + "config.cfg";
    version = ver;

    printf("Config(%s, v%i)\n", configPath.c_str(), ver);

    /// add default roms paths
    roms_paths.clear();
    roms_paths.emplace_back(io->getDataPath() + "roms/");

    /// default options available for all cores
    /////////////////////////////////////////////////
    /// main/gui config
    /////////////////////////////////////////////////
    append("MAIN", {"MAIN"}, 0, Option::Id::MENU_MAIN, Option::Flags::MENU);
    append("SHOW", {"ALL", "AVAILABLE", "FAVORITES"}, 0, Option::Id::GUI_SHOW_ALL, Option::Flags::STRING);
    append("SHOW_CLONES", {"OFF", "ON"}, 0,
           Option::Id::GUI_FILTER_CLONES, Option::Flags::BOOLEAN | Option::Flags::HIDDEN);
    append("SHOW_ZIP_NAMES", {"OFF", "ON"}, 1, Option::Id::GUI_SHOW_ZIP_NAMES, Option::Flags::BOOLEAN);
    append("SHOW_ICONS", {"OFF", "ON"}, 0, Option::Id::GUI_SHOW_ICONS, Option::Flags::BOOLEAN | Option::Flags::HIDDEN);
    get()->at(get()->size() - 1).setInfo("YOU NEED TO RESTART THE APPLICATION AFTER CHANGING THIS OPTION");
    append("SCREEN_WIDTH", C2D_SCREEN_WIDTH, Option::Id::GUI_SCREEN_WIDTH,
           Option::Flags::INTEGER | Option::Flags::HIDDEN);
    append("SCREEN_HEIGHT", C2D_SCREEN_HEIGHT, Option::Id::GUI_SCREEN_HEIGHT,
           Option::Flags::INTEGER | Option::Flags::HIDDEN);
#ifdef __FULLSCREEN__
    append("FULLSCREEN", {"OFF", "ON"}, 0, Option::Id::GUI_FULLSCREEN, Option::Flags::BOOLEAN);
    get()->at(get()->size() - 1).setInfo("YOU NEED TO RESTART THE APPLICATION AFTER CHANGING THIS OPTION");
#endif

    // build  skin list
    std::vector<std::string> skins;
    // add default skins from romfs
    skins.emplace_back("default");
    skins.emplace_back("big_preview");
    // add skins from data dir
    auto files = io->getDirList(dataPath + "skins/", true);
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

    // set default skin index
    if (!get(Option::Id::GUI_SKIN)) {
        int index = 0;
        for (size_t i = 0; i < skins.size(); i++) {
            if (skins.at(i) == "default") {
                index = (int) i;
            }
        }
        append("SKIN", skins, index, Option::Id::GUI_SKIN, Option::Flags::STRING);
    }
    get()->at(get()->size() - 1).setInfo("YOU NEED TO RESTART THE APPLICATION AFTER CHANGING THIS OPTION");

    append("VIDEO_SNAP_DELAY", 5, Option::Id::GUI_VIDEO_SNAP_DELAY, Option::Flags::INTEGER);
#ifdef __SWITCH__
    append("SINGLE_JOYCONS", {"OFF", "ON"}, 0, Option::Id::JOY_SINGLEJOYCON, Option::Flags::BOOLEAN);
#endif

    /////////////////////////////////////////////////
    /// default rom config
    /////////////////////////////////////////////////
    append("EMULATION", {"EMULATION"}, 0, Option::Id::MENU_ROM_OPTIONS, Option::Flags::MENU);
    if (C2D_SCREEN_HEIGHT > 720) {
        append("SCALING", {"NONE", "2X", "3X", "4X", "FIT", "FULL"}, 3,
               Option::Id::ROM_SCALING, Option::Flags::STRING);
    } else if (C2D_SCREEN_HEIGHT > 240) {
        append("SCALING", {"NONE", "2X", "3X", "FIT", "FULL"}, 2,
               Option::Id::ROM_SCALING, Option::Flags::STRING);
    } else {
        append("SCALING", {"NONE", "FIT", "FULL"}, 0, Option::Id::ROM_SCALING, Option::Flags::STRING);
    }
    append("SCALING_MODE", {"AUTO", "ASPECT", "INTEGER"}, 0,
           Option::Id::ROM_SCALING_MODE, Option::Flags::STRING);
    append("FILTER", {"POINT", "LINEAR"}, 0, Option::Id::ROM_FILTER, Option::Flags::STRING);
    append("SHOW_FPS", {"OFF", "ON"}, 0, Option::Id::ROM_SHOW_FPS, Option::Flags::BOOLEAN);

    /// joysticks config
    append("JOYPAD", {"JOYPAD"}, 0, Option::Id::MENU_JOYPAD, Option::Flags::MENU);
    append("JOY_UP", KEY_JOY_UP_DEFAULT, Option::Id::JOY_UP, Option::Flags::INPUT);
    append("JOY_DOWN", KEY_JOY_DOWN_DEFAULT, Option::Id::JOY_DOWN, Option::Flags::INPUT);
    append("JOY_LEFT", KEY_JOY_LEFT_DEFAULT, Option::Id::JOY_LEFT, Option::Flags::INPUT);
    append("JOY_RIGHT", KEY_JOY_RIGHT_DEFAULT, Option::Id::JOY_RIGHT, Option::Flags::INPUT);
    append("JOY_FIRE1", KEY_JOY_FIRE1_DEFAULT, Option::Id::JOY_FIRE1, Option::Flags::INPUT);
    append("JOY_FIRE2", KEY_JOY_FIRE2_DEFAULT, Option::Id::JOY_FIRE2, Option::Flags::INPUT);
    append("JOY_FIRE3", KEY_JOY_FIRE3_DEFAULT, Option::Id::JOY_FIRE3, Option::Flags::INPUT);
    append("JOY_FIRE4", KEY_JOY_FIRE4_DEFAULT, Option::Id::JOY_FIRE4, Option::Flags::INPUT);
    append("JOY_FIRE5", KEY_JOY_FIRE5_DEFAULT, Option::Id::JOY_FIRE5, Option::Flags::INPUT);
    append("JOY_FIRE6", KEY_JOY_FIRE6_DEFAULT, Option::Id::JOY_FIRE6, Option::Flags::INPUT);
    append("JOY_FIRE7", KEY_JOY_FIRE7_DEFAULT, Option::Id::JOY_FIRE7, Option::Flags::INPUT);
    append("JOY_FIRE8", KEY_JOY_FIRE8_DEFAULT, Option::Id::JOY_FIRE8, Option::Flags::INPUT);
    append("JOY_COIN1", KEY_JOY_COIN1_DEFAULT, Option::Id::JOY_COIN1, Option::Flags::INPUT);
    append("JOY_START1", KEY_JOY_START1_DEFAULT, Option::Id::JOY_START1, Option::Flags::INPUT);
    append("JOY_MENU1", KEY_JOY_MENU1_DEFAULT, Option::Id::JOY_MENU1, Option::Flags::INPUT);
    append("JOY_MENU2", KEY_JOY_MENU2_DEFAULT, Option::Id::JOY_MENU2, Option::Flags::INPUT);
    // TODO: add gui option for axis in option menu
    append("JOY_AXIS_LX", KEY_JOY_AXIS_LX, Option::Id::JOY_AXIS_LX, Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_AXIS_LY", KEY_JOY_AXIS_LY, Option::Id::JOY_AXIS_LY, Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_AXIS_RX", KEY_JOY_AXIS_RX, Option::Id::JOY_AXIS_RX, Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_AXIS_RY", KEY_JOY_AXIS_RY, Option::Id::JOY_AXIS_RY, Option::Flags::INPUT | Option::Flags::HIDDEN);
    append("JOY_DEADZONE",
           {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
            "18000", "20000", "22000", "24000", "26000", "28000", "30000"}, 3, Option::Id::JOY_DEADZONE,
           Option::Flags::INTEGER);
#ifndef NO_KEYBOARD
    // keyboard
    append("KEYBOARD", {"KEYBOARD"}, 0, Option::Id::MENU_KEYBOARD, Option::Flags::MENU);
    append("KEY_UP", KEY_KB_UP_DEFAULT, Option::Id::KEY_UP, Option::Flags::INPUT);
    append("KEY_DOWN", KEY_KB_DOWN_DEFAULT, Option::Id::KEY_DOWN, Option::Flags::INPUT);
    append("KEY_LEFT", KEY_KB_LEFT_DEFAULT, Option::Id::KEY_LEFT, Option::Flags::INPUT);
    append("KEY_RIGHT", KEY_KB_RIGHT_DEFAULT, Option::Id::KEY_RIGHT, Option::Flags::INPUT);
    append("KEY_FIRE1", KEY_KB_FIRE1_DEFAULT, Option::Id::KEY_FIRE1, Option::Flags::INPUT);
    append("KEY_FIRE2", KEY_KB_FIRE2_DEFAULT, Option::Id::KEY_FIRE2, Option::Flags::INPUT);
    append("KEY_FIRE3", KEY_KB_FIRE3_DEFAULT, Option::Id::KEY_FIRE3, Option::Flags::INPUT);
    append("KEY_FIRE4", KEY_KB_FIRE4_DEFAULT, Option::Id::KEY_FIRE4, Option::Flags::INPUT);
    append("KEY_FIRE5", KEY_KB_FIRE5_DEFAULT, Option::Id::KEY_FIRE5, Option::Flags::INPUT);
    append("KEY_FIRE6", KEY_KB_FIRE6_DEFAULT, Option::Id::KEY_FIRE6, Option::Flags::INPUT);
    append("KEY_FIRE7", KEY_KB_FIRE7_DEFAULT, Option::Id::KEY_FIRE7, Option::Flags::INPUT);
    append("KEY_FIRE8", KEY_KB_FIRE8_DEFAULT, Option::Id::KEY_FIRE8, Option::Flags::INPUT);
    append("KEY_COIN1", KEY_KB_COIN1_DEFAULT, Option::Id::KEY_COIN1, Option::Flags::INPUT);
    append("KEY_START1", KEY_KB_START1_DEFAULT, Option::Id::KEY_START1, Option::Flags::INPUT);
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
                    for (size_t i = 0; i < roms_paths.size(); i++) {
                        char p[MAX_PATH];
                        snprintf(p, MAX_PATH, "ROMS_PATH%i", (int) i);
                        const char *value = nullptr;
                        if (config_setting_lookup_string(settings, p, &value) != 0) {
                            roms_paths[i] = value;
                            if (!roms_paths[i].empty() && roms_paths[i].back() != '/')
                                roms_paths[i] += '/';
                            printf("%s: %s\n", p, value);
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
                        if (config_setting_lookup_int(settings, option.getName().c_str(), &value) != 0) {
                            option.setValueInt(value);
                            //printf("Config::load: OPTION: %s, VALUE: %i\n", option.getName().c_str(),
                            //       option.getValueInt());
                        }
                    } else {
                        const char *value;
                        if (config_setting_lookup_string(settings, option.getName().c_str(), &value) != 0) {
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
            sub_setting = config_setting_add(setting_fba, option.getName().c_str(), CONFIG_TYPE_GROUP);
            continue;
        }

        if (flags & Option::Flags::INTEGER || flags & Option::Flags::INPUT) {
            config_setting_t *setting = config_setting_add(sub_setting, option.getName().c_str(), CONFIG_TYPE_INT);
            config_setting_set_int(setting, option.getValueInt());
        } else {
            config_setting_t *setting = config_setting_add(sub_setting, option.getName().c_str(), CONFIG_TYPE_STRING);
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

bool Config::add(int target,
                 const std::string &text, const std::vector<std::string> &values,
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

int *Config::getPlayerInputKeys(int player, bool isRom) {
#ifndef NO_KEYBOARD
    // TODO: allow per player config
    keyboard_keys[0] = get(Option::Id::KEY_UP, isRom)->getValueInt();
    keyboard_keys[1] = get(Option::Id::KEY_DOWN, isRom)->getValueInt();
    keyboard_keys[2] = get(Option::Id::KEY_LEFT, isRom)->getValueInt();
    keyboard_keys[3] = get(Option::Id::KEY_RIGHT, isRom)->getValueInt();
    keyboard_keys[4] = get(Option::Id::KEY_COIN1, isRom)->getValueInt();
    keyboard_keys[5] = get(Option::Id::KEY_START1, isRom)->getValueInt();
    keyboard_keys[6] = get(Option::Id::KEY_FIRE1, isRom)->getValueInt();
    keyboard_keys[7] = get(Option::Id::KEY_FIRE2, isRom)->getValueInt();
    keyboard_keys[8] = get(Option::Id::KEY_FIRE3, isRom)->getValueInt();
    keyboard_keys[9] = get(Option::Id::KEY_FIRE4, isRom)->getValueInt();
    keyboard_keys[10] = get(Option::Id::KEY_FIRE5, isRom)->getValueInt();
    keyboard_keys[11] = get(Option::Id::KEY_FIRE6, isRom)->getValueInt();
    keyboard_keys[12] = get(Option::Id::KEY_FIRE7, isRom)->getValueInt();
    keyboard_keys[13] = get(Option::Id::KEY_FIRE8, isRom)->getValueInt();
    keyboard_keys[14] = get(Option::Id::KEY_MENU1, isRom)->getValueInt();
    keyboard_keys[15] = get(Option::Id::KEY_MENU2, isRom)->getValueInt();
#endif

    return keyboard_keys;
}

int *Config::getPlayerInputButtons(int player, bool isRom) {
    // TODO: allow per player config
    joystick_keys[0] = get(Option::Id::JOY_UP, isRom)->getValueInt();
    joystick_keys[1] = get(Option::Id::JOY_DOWN, isRom)->getValueInt();
    joystick_keys[2] = get(Option::Id::JOY_LEFT, isRom)->getValueInt();
    joystick_keys[3] = get(Option::Id::JOY_RIGHT, isRom)->getValueInt();
    joystick_keys[4] = get(Option::Id::JOY_COIN1, isRom)->getValueInt();
    joystick_keys[5] = get(Option::Id::JOY_START1, isRom)->getValueInt();
    joystick_keys[6] = get(Option::Id::JOY_FIRE1, isRom)->getValueInt();
    joystick_keys[7] = get(Option::Id::JOY_FIRE2, isRom)->getValueInt();
    joystick_keys[8] = get(Option::Id::JOY_FIRE3, isRom)->getValueInt();
    joystick_keys[9] = get(Option::Id::JOY_FIRE4, isRom)->getValueInt();
    joystick_keys[10] = get(Option::Id::JOY_FIRE5, isRom)->getValueInt();
    joystick_keys[11] = get(Option::Id::JOY_FIRE6, isRom)->getValueInt();
    joystick_keys[12] = get(Option::Id::JOY_FIRE7, isRom)->getValueInt();
    joystick_keys[13] = get(Option::Id::JOY_FIRE8, isRom)->getValueInt();
    joystick_keys[14] = get(Option::Id::JOY_MENU1, isRom)->getValueInt();
    joystick_keys[15] = get(Option::Id::JOY_MENU2, isRom)->getValueInt();
    // axis
    joystick_keys[16] = get(Option::Id::JOY_AXIS_LX, isRom)->getValueInt();
    joystick_keys[17] = get(Option::Id::JOY_AXIS_LY, isRom)->getValueInt();
    joystick_keys[18] = get(Option::Id::JOY_AXIS_RX, isRom)->getValueInt();
    joystick_keys[19] = get(Option::Id::JOY_AXIS_RY, isRom)->getValueInt();

    return joystick_keys;
}

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

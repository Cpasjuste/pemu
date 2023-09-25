//
// Created by cpasjuste on 13/09/23.
//

#include "pemu.h"
#include "pemu_config.h"

using namespace c2d::config;
using namespace ss_api;

PEMUConfig::PEMUConfig(c2d::Io *io, const std::string &name, int version)
        : Config(name, io->getDataPath() + "config.cfg", version) {
    p_io = io;

    /*
     * ROMS PATHS
     */
    Group roms("ROMS", CFG_ID_ROMS);
    addGroup(roms);

    /*
     * UI
     */
    Group main("UI", Id::MENU_MAIN);
    main.addOption({"SHOW_FAVORITES", {"OFF", "ON"}, 0, Id::GUI_SHOW_FAVORITES});
    main.addOption({"SHOW_AVAILABLE", {"OFF", "ON"}, 0, Id::GUI_SHOW_AVAILABLE});
    main.addOption({"SHOW_CLONES", {"OFF", "ON"}, 0, Id::GUI_FILTER_CLONES})->setFlags(Flags::HIDDEN);
    main.addOption({"SHOW_ZIP_NAMES", {"OFF", "ON"}, 1, Id::GUI_SHOW_ZIP_NAMES});
#ifdef __FULLSCREEN__
    main.addOption({"FULLSCREEN", {"OFF", "ON"}, 1, Id::GUI_FULLSCREEN, C2D_CONFIG_RESTART_NEEDED});
#endif
    Vector2i displaySize = C2DDevice::getDisplaySize();
    int aspect_index = (float) displaySize.x / (float) displaySize.y > 1.33f ? 0 : 1;
    main.addOption({"SKIN_ASPECT", {"16/9", "4/3"}, aspect_index,
                    Id::GUI_SKIN_ASPECT, C2D_CONFIG_RESTART_NEEDED});
    main.addOption({"FONT_SCALING", {"0", "1", "2", "3", "4", "5"}, 0,
                    Id::GUI_FONT_SCALING, C2D_CONFIG_RESTART_NEEDED});
    main.addOption({"VIDEO_SNAP_DELAY", {"3", "5", "7", "10"}, 1, Id::GUI_VIDEO_SNAP_DELAY});

    // build  skin list
    std::vector<std::string> skins;
    // add default skins from romfs
    skins.emplace_back("default");
    // add skins from romfs dir
    auto files = io->getDirList(io->getRomFsPath() + "skins/", true);
    for (auto &file: files) {
        if (file.type != c2d::Io::Type::Directory || file.name[0] == '.') {
            continue;
        }
        // only append skin name if not found yet
        if (std::find(skins.begin(), skins.end(), file.name) == skins.end()) {
            skins.emplace_back(file.name);
            printf("skin found: %s\n", file.path.c_str());
        }
    }
    // add skins from data dir
    files = io->getDirList(io->getDataPath() + "skins/", true);
    for (auto &file: files) {
        if (file.type != c2d::Io::Type::Directory || file.name[0] == '.') {
            continue;
        }
        // only append skin name if not found yet
        if (std::find(skins.begin(), skins.end(), file.name) == skins.end()) {
            skins.emplace_back(file.name);
            printf("skin found: %s\n", file.path.c_str());
        }
    }
    // set "default" skin index
    int index = 0;
    for (size_t i = 0; i < skins.size(); i++) {
        if (skins.at(i) == "default") {
            index = (int) i;
        }
    }
    main.addOption({"SKIN", skins, index, PEMUConfig::Id::GUI_SKIN});
    addGroup(main);

    /*
     * Emulators options
     */
    Group emu_grp("EMULATION", Id::MENU_ROM_OPTIONS);
    if (displaySize.y > 1080) {
        emu_grp.addOption(
                {"SCALING", {"NONE", "2X", "3X", "4X", "5X", "6X", "7X", "8X", "9X", "FIT", "FULL"}, 6,
                 Id::ROM_SCALING});
    } else if (displaySize.y > 720) {
        emu_grp.addOption({"SCALING", {"NONE", "2X", "3X", "4X", "FIT", "FULL"}, 4, Id::ROM_SCALING});
    } else if (displaySize.y > 544) {
        emu_grp.addOption({"SCALING", {"NONE", "2X", "3X", "FIT", "FULL"}, 3, Id::ROM_SCALING});
    } else if (displaySize.y > 240) {
        emu_grp.addOption({"SCALING", {"NONE", "2X", "FIT", "FULL"}, 2, Id::ROM_SCALING});
    } else {
        emu_grp.addOption({"SCALING", {"NONE", "FIT", "FULL"}, 1, Id::ROM_SCALING});
    }

    emu_grp.addOption({"SCALING_MODE", {"AUTO", "ASPECT", "INTEGER"}, 1, Id::ROM_SCALING_MODE});
    emu_grp.addOption({"FILTER", {std::string("POINT"), std::string("LINEAR")}, 1, Id::ROM_FILTER});
#ifdef __VITA__
    emu_grp.addOption({"WAIT_RENDERING", {"OFF", "ON"}, 1, Id::ROM_WAIT_RENDERING});
#endif
    emu_grp.addOption({"SHOW_FPS", {"OFF", "ON"}, 0, Id::ROM_SHOW_FPS});
    addGroup(emu_grp);

    /*
     * Inputs options
     */
    Group joy_grp("GAMEPAD", Id::MENU_JOYPAD);
    joy_grp.addOption({"JOY_UP", KEY_JOY_UP_DEFAULT, Id::JOY_UP})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_DOWN", KEY_JOY_DOWN_DEFAULT, Id::JOY_DOWN})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_LEFT", KEY_JOY_LEFT_DEFAULT, Id::JOY_LEFT})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_RIGHT", KEY_JOY_RIGHT_DEFAULT, Id::JOY_RIGHT})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_A", KEY_JOY_A_DEFAULT, Id::JOY_A})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_B", KEY_JOY_B_DEFAULT, Id::JOY_B})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_X", KEY_JOY_X_DEFAULT, Id::JOY_X})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_Y", KEY_JOY_Y_DEFAULT, Id::JOY_Y})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_LT", KEY_JOY_LT_DEFAULT, Id::JOY_LT})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_RT", KEY_JOY_RT_DEFAULT, Id::JOY_RT})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_LB", KEY_JOY_LB_DEFAULT, Id::JOY_LB})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_RB", KEY_JOY_RB_DEFAULT, Id::JOY_RB})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_SELECT", KEY_JOY_SELECT_DEFAULT, Id::JOY_SELECT})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_START", KEY_JOY_START_DEFAULT, Id::JOY_START})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_MENU1", KEY_JOY_MENU1_DEFAULT, Id::JOY_MENU1})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_MENU2", KEY_JOY_MENU2_DEFAULT, Id::JOY_MENU2})->setFlags(Flags::INPUT);
    joy_grp.addOption({"JOY_AXIS_LX", KEY_JOY_AXIS_LX, Id::JOY_AXIS_LX})->setFlags(Flags::INPUT | Flags::HIDDEN);
    joy_grp.addOption({"JOY_AXIS_LY", KEY_JOY_AXIS_LY, Id::JOY_AXIS_LY})->setFlags(Flags::INPUT | Flags::HIDDEN);
    joy_grp.addOption({"JOY_AXIS_RX", KEY_JOY_AXIS_RX, Id::JOY_AXIS_RX})->setFlags(Flags::INPUT | Flags::HIDDEN);
    joy_grp.addOption({"JOY_AXIS_RY", KEY_JOY_AXIS_RY, Id::JOY_AXIS_RY})->setFlags(Flags::INPUT | Flags::HIDDEN);
    joy_grp.addOption({"JOY_DEADZONE", {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
                                        "18000", "20000", "22000", "24000", "26000", "28000", "30000"},
                       3, Id::JOY_DEADZONE});
    addGroup(joy_grp);
#ifndef NO_KEYBOARD
    Group kb_grp("KEYBOARD", Id::MENU_KEYBOARD);
    kb_grp.addOption({"KEY_UP", KEY_KB_UP_DEFAULT, Id::KEY_UP})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_DOWN", KEY_KB_DOWN_DEFAULT, Id::KEY_DOWN})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_LEFT", KEY_KB_LEFT_DEFAULT, Id::KEY_LEFT})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_RIGHT", KEY_KB_RIGHT_DEFAULT, Id::KEY_RIGHT})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_A", KEY_KB_A_DEFAULT, Id::KEY_A})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_B", KEY_KB_B_DEFAULT, Id::KEY_B})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_X", KEY_KB_X_DEFAULT, Id::KEY_X})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_Y", KEY_KB_Y_DEFAULT, Id::KEY_Y})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_LT", KEY_KB_LT_DEFAULT, Id::KEY_LT})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_RT", KEY_KB_RT_DEFAULT, Id::KEY_RT})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_LB", KEY_KB_LB_DEFAULT, Id::KEY_LB})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_RB", KEY_KB_RB_DEFAULT, Id::KEY_RB})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_SELECT", KEY_KB_SELECT_DEFAULT, Id::KEY_SELECT})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_START", KEY_KB_START_DEFAULT, Id::KEY_START})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_MENU1", KEY_KB_MENU1_DEFAULT, Id::KEY_MENU1})->setFlags(Flags::INPUT);
    kb_grp.addOption({"KEY_MENU2", KEY_KB_MENU2_DEFAULT, Id::KEY_MENU2})->setFlags(Flags::INPUT);
    addGroup(kb_grp);
#endif
}

bool PEMUConfig::loadGame(const Game &game) {
    Group group;

    // free game config if needed
    delete (p_game_config);

    // create game configuration
    std::string path = p_io->getDataPath() + "configs/" + Utility::removeExt(game.path) + ".cfg";
    p_game_config = new Config(getName() + "_GAME", path, getAppVersion());

    // copy game emulation config section from main config for default options
    getGroup(Id::MENU_ROM_OPTIONS)->copy(&group);
    p_game_config->addGroup(group);

    // copy game gamepad config from main config for default options
    getGroup(Id::MENU_JOYPAD)->copy(&group);
    p_game_config->addGroup(group);

#ifndef NO_KEYBOARD
    // copy game keyboard config from main config for default options
    getGroup(Id::MENU_KEYBOARD)->copy(&group);
    p_game_config->addGroup(group);
#endif

    p_game_config->load();

    return true;
}

bool PEMUConfig::save(bool isGame) {
    if (isGame) {
        if (!p_game_config) return false;
        return p_game_config->save();
    }

    return Config::save();
}

c2d::config::Option *PEMUConfig::getOption(int id, bool isGame) {
    if (isGame && p_game_config) {
        return p_game_config->getOption(id);
    }

    return Config::getOption(id);
}

c2d::config::Option *PEMUConfig::get(int id, bool isGame) {
    return getOption(id, isGame);
}

std::string PEMUConfig::getRomPath(const std::string &name) {
    auto group = getGroup(CFG_ID_ROMS);
    if (!group) return "";

    auto option = name.empty() ? &group->getOptions()->at(0) : group->getOption(name);
    if (!option) return "";
    return option->getString();
}

std::vector<c2d::Input::ButtonMapping> PEMUConfig::getKeyboardMapping(int player, bool isGame) {
#ifndef NO_KEYBOARD
    return {
            {Input::Button::Up,     getOption(Id::KEY_UP, isGame)->getInteger()},
            {Input::Button::Down,   getOption(Id::KEY_DOWN, isGame)->getInteger()},
            {Input::Button::Left,   getOption(Id::KEY_LEFT, isGame)->getInteger()},
            {Input::Button::Right,  getOption(Id::KEY_RIGHT, isGame)->getInteger()},
            {Input::Button::Select, getOption(Id::KEY_SELECT, isGame)->getInteger()},
            {Input::Button::Start,  getOption(Id::KEY_START, isGame)->getInteger()},
            {Input::Button::A,      getOption(Id::KEY_A, isGame)->getInteger()},
            {Input::Button::B,      getOption(Id::KEY_B, isGame)->getInteger()},
            {Input::Button::X,      getOption(Id::KEY_X, isGame)->getInteger()},
            {Input::Button::Y,      getOption(Id::KEY_Y, isGame)->getInteger()},
            {Input::Button::LT,     getOption(Id::KEY_LT, isGame)->getInteger()},
            {Input::Button::RT,     getOption(Id::KEY_RT, isGame)->getInteger()},
            {Input::Button::LB,     getOption(Id::KEY_LB, isGame)->getInteger()},
            {Input::Button::RB,     getOption(Id::KEY_RB, isGame)->getInteger()},
            {Input::Button::Menu1,  getOption(Id::KEY_MENU1, isGame)->getInteger()},
            {Input::Button::Menu2,  getOption(Id::KEY_MENU2, isGame)->getInteger()}
    };
#else
    return {};
#endif
}

std::vector<c2d::Input::ButtonMapping> PEMUConfig::getJoystickMapping(int player, bool isGame) {
    // TODO: allow per player config
    return {
            {Input::Button::Up,     getOption(Id::JOY_UP, isGame)->getInteger()},
            {Input::Button::Down,   getOption(Id::JOY_DOWN, isGame)->getInteger()},
            {Input::Button::Left,   getOption(Id::JOY_LEFT, isGame)->getInteger()},
            {Input::Button::Right,  getOption(Id::JOY_RIGHT, isGame)->getInteger()},
            {Input::Button::Select, getOption(Id::JOY_SELECT, isGame)->getInteger()},
            {Input::Button::Start,  getOption(Id::JOY_START, isGame)->getInteger()},
            {Input::Button::A,      getOption(Id::JOY_A, isGame)->getInteger()},
            {Input::Button::B,      getOption(Id::JOY_B, isGame)->getInteger()},
            {Input::Button::X,      getOption(Id::JOY_X, isGame)->getInteger()},
            {Input::Button::Y,      getOption(Id::JOY_Y, isGame)->getInteger()},
            {Input::Button::LT,     getOption(Id::JOY_LT, isGame)->getInteger()},
            {Input::Button::RT,     getOption(Id::JOY_RT, isGame)->getInteger()},
            {Input::Button::LB,     getOption(Id::JOY_LB, isGame)->getInteger()},
            {Input::Button::RB,     getOption(Id::JOY_RB, isGame)->getInteger()},
            {Input::Button::Menu1,  getOption(Id::JOY_MENU1, isGame)->getInteger()},
            {Input::Button::Menu2,  getOption(Id::JOY_MENU2, isGame)->getInteger()}
    };
}

c2d::Vector2i PEMUConfig::getJoystickAxisLeftMapping(int player, bool isGame) {
    return {
            getOption(Id::JOY_AXIS_LX, isGame)->getInteger(),
            getOption(Id::JOY_AXIS_LY, isGame)->getInteger()
    };
}

c2d::Vector2i PEMUConfig::getJoystickAxisRightMapping(int player, bool isGame) {
    return {
            getOption(Id::JOY_AXIS_RX, isGame)->getInteger(),
            getOption(Id::JOY_AXIS_RY, isGame)->getInteger()
    };
}

int PEMUConfig::getJoystickDeadZone(int player, bool isGame) {
    return getOption(Id::JOY_DEADZONE, isGame)->getInteger();
}

PEMUConfig::~PEMUConfig() {
    printf("PEMUConfig::~PEMUConfig()\n");
    delete (p_game_config);
}

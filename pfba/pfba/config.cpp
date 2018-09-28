//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include <vector>

#include "burner.h"

#include "c2dui.h"
#include "config.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

PFBAConfig::PFBAConfig(Renderer *renderer, const std::string &home, int version)
        : Config(home, version) {

    // add default roms paths
    getRomPaths()->emplace_back(home + "coleco/");
    getRomPaths()->emplace_back(home + "gamegear/");
    getRomPaths()->emplace_back(home + "megadriv/");
    getRomPaths()->emplace_back(home + "msx/");
    getRomPaths()->emplace_back(home + "pce/");
    getRomPaths()->emplace_back(home + "sg1000/");
    getRomPaths()->emplace_back(home + "sgx/");
    getRomPaths()->emplace_back(home + "sms/");
    getRomPaths()->emplace_back(home + "tg16/");
    for (size_t i = getRomPaths()->size(); i < C2DUI_ROMS_PATHS_MAX; i++) {
        getRomPaths()->emplace_back(std::string());
    }

    // add hardware list
    // getHardwareList()->emplace_back(HARDWARE_PREFIX_ALL, "All");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SNK, "Neo Geo");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CAPCOM, "CPS-1");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CPS2, "CPS-2");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CPS3, "CPS-3");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_IGS_PGM, "PGM");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_PSIKYO, "Psikyo");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CAVE, "Cave");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA, "Sega");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_CAPCOM_MISC, "Capcom (Other)");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_DATAEAST, "Data East");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_GALAXIAN, "Galaxian");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_IREM, "Irem");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_KANEKO, "Kaneko");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_KONAMI, "Konami");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MISC_PRE90S, "Misc (pre 90s)");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MISC_POST90S, "Misc (post 90s)");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_PACMAN, "Pacman");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SETA, "Seta");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_TAITO, "Taito");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_TECHNOS, "Technos");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_TOAPLAN, "Toaplan");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_MEGADRIVE, "Sega MegaDrive");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_MASTER_SYSTEM, "Sega MasterSystem");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_SG1000, "Sega SG1000");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_SEGA_GAME_GEAR, "Sega GameGear");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_PCENGINE, "PC-Engine");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_COLECO, "Coleco");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MIDWAY, "Midway");
    getHardwareList()->emplace_back(HARDWARE_PREFIX_MSX, "MSX");
    std::vector<std::string> hardware_names;
    for (auto &i : *getHardwareList()) {
        hardware_names.emplace_back(i.name);
    }

    // TODO: change this crap
    ////////////////////////////////////////////////////////////
    // options needs to be in sync/order with "Option::Index"
    ////////////////////////////////////////////////////////////
    // main/gui config
    get()->emplace_back(
            Option("MAIN", {"MAIN"}, 0, Option::Index::MENU_MAIN, Option::Type::MENU));
    get()->emplace_back(Option("SHOW_ALL", {"WORKING", "ALL"}, 1, Option::Index::GUI_SHOW_ALL));
    get()->emplace_back(Option("SHOW_CLONES", {"OFF", "ON"}, 0, Option::Index::GUI_SHOW_CLONES));
    get()->emplace_back(Option("SHOW_HARDWARE", hardware_names, 0, Option::Index::GUI_SHOW_HARDWARE));
#ifdef __PSP2__
    get()->emplace_back(
            Option("SHOW_ICONS", {"OFF", "ON"}, 0, Option::Index::GUI_SHOW_ICONS, Option::Type::HIDDEN));
#else
    get()->emplace_back(Option("SHOW_ICONS", {"OFF", "ON"}, 0, Option::Index::GUI_SHOW_ICONS));
#endif
    get()->emplace_back(
            Option("FULLSCREEN", {"OFF", "ON"}, 1, Option::Index::GUI_FULLSCREEN, Option::Type::HIDDEN));
#ifdef __SWITCH__
    get()->emplace_back(Option("SINGLE_JOYCONS", {"OFF", "ON"}, 0, Option::Index::JOY_SINGLEJOYCON));
#endif

    // skin config, hidden in gui for now
    get()->emplace_back(
            Option("SKIN", {"SKIN"}, 0, Option::Index::MENU_SKIN,
                        Option::Type::MENU | Option::Type::HIDDEN));
    get()->emplace_back(
            Option("SKIN_FONT_SIZE", {"20"}, 20, Option::Index::SKIN_FONT_SIZE, Option::Type::HIDDEN));
    // default rom config
    get()->emplace_back(
            Option("EMULATION", {"EMULATION"}, 0, Option::Index::MENU_ROM_OPTIONS, Option::Type::MENU));
    get()->emplace_back(
            Option("SCALING", {"NONE", "2X", "3X", "FIT", "FIT 4:3", "FULL"},
                        4, Option::Index::ROM_SCALING));
    get()->emplace_back(
            Option("FILTER", {"POINT", "LINEAR"}, 0, Option::Index::ROM_FILTER));
    if (renderer->getShaderList() != nullptr) {
        get()->emplace_back(
                Option("EFFECT", renderer->getShaderList()->getNames(), 0, Option::Index::ROM_SHADER));
    }
#ifdef __PSP2__
    get()->emplace_back(
            Option("ROTATION", {"OFF", "ON", "FLIP", "CAB MODE"}, 0, Option::Index::ROM_ROTATION));
#else
    get()->emplace_back(
            Option("ROTATION", {"OFF", "ON", "FLIP"}, 0, Option::Index::ROM_ROTATION));
#endif
    get()->emplace_back(Option("SHOW_FPS", {"OFF", "ON"}, 0, Option::Index::ROM_SHOW_FPS));
    get()->emplace_back(Option("FRAMESKIP", {"OFF", "1", "2", "3", "4", "5", "6", "7", "8", "9"},
                                           0, Option::Index::ROM_FRAMESKIP, Option::Type::INTEGER));
    //get()->emplace_back(Option("M68K", {"ASM", "C"}, 0, Option::Index::ROM_M68K));
    get()->emplace_back(
            Option("NEOBIOS", {"UNIBIOS_3_2", "AES_ASIA", "AES_JPN", "DEVKIT", "MVS_ASIA_EUR_V6S1",
                                    "MVS_ASIA_EUR_V5S1", "MVS_ASIA_EUR_V3S4", "MVS_USA_V5S2",
                                    "MVS_USA_V5S4", "MVS_USA_V5S6", "MVS_JPN_V6", "MVS_JPN_V5",
                                    "MVS_JPN_V3S4", "NEO_MVH_MV1C", "MVS_JPN_J3", "DECK_V6"},
                        0, Option::Index::ROM_NEOBIOS));
    get()->emplace_back(
            Option("AUDIO", {"OFF", "ON"}, 1, Option::Index::ROM_AUDIO, Option::Type::HIDDEN));

    // joystick
    get()->emplace_back(
            Option("JOYPAD", {"JOYPAD"}, 0, Option::Index::MENU_JOYPAD, Option::Type::MENU));
    get()->emplace_back(
            Option("JOY_UP", {"-1"}, KEY_JOY_UP_DEFAULT, Option::Index::JOY_UP, Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_DOWN", {"-1"}, KEY_JOY_DOWN_DEFAULT, Option::Index::JOY_DOWN,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_LEFT", {"-1"}, KEY_JOY_LEFT_DEFAULT, Option::Index::JOY_LEFT,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_RIGHT", {"-1"}, KEY_JOY_RIGHT_DEFAULT, Option::Index::JOY_RIGHT,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_FIRE1", {"0"}, KEY_JOY_FIRE1_DEFAULT, Option::Index::JOY_FIRE1,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_FIRE2", {"1"}, KEY_JOY_FIRE2_DEFAULT, Option::Index::JOY_FIRE2,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_FIRE3", {"2"}, KEY_JOY_FIRE3_DEFAULT, Option::Index::JOY_FIRE3,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_FIRE4", {"3"}, KEY_JOY_FIRE4_DEFAULT, Option::Index::JOY_FIRE4,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_FIRE5", {"4"}, KEY_JOY_FIRE5_DEFAULT, Option::Index::JOY_FIRE5,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_FIRE6", {"5"}, KEY_JOY_FIRE6_DEFAULT, Option::Index::JOY_FIRE6,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_COIN1", {"6"}, KEY_JOY_COIN1_DEFAULT, Option::Index::JOY_COIN1,
                        Option::Type::INPUT));
    get()->emplace_back(
            Option("JOY_START1", {"7"}, KEY_JOY_START1_DEFAULT, Option::Index::JOY_START1,
                        Option::Type::INPUT));
    // TODO: add gui option for axis in option menu
    get()->emplace_back(
            Option("JOY_AXIS_LX", {"0"}, KEY_JOY_AXIS_LX, Option::Index::JOY_AXIS_LX,
                        Option::Type::HIDDEN));
    get()->emplace_back(
            Option("JOY_AXIS_LY", {"1"}, KEY_JOY_AXIS_LY, Option::Index::JOY_AXIS_LY,
                        Option::Type::HIDDEN));
    get()->emplace_back(
            Option("JOY_AXIS_RX", {"2"}, KEY_JOY_AXIS_RX, Option::Index::JOY_AXIS_RX,
                        Option::Type::HIDDEN));
    get()->emplace_back(
            Option("JOY_AXIS_RY", {"3"}, KEY_JOY_AXIS_RY, Option::Index::JOY_AXIS_RY,
                        Option::Type::HIDDEN));
    get()->emplace_back(Option("JOY_DEADZONE",
                                           {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
                                            "18000", "20000", "22000", "24000", "26000", "28000", "30000"}, 3,
                                           Option::Index::JOY_DEADZONE, Option::Type::INTEGER));

#ifndef NO_KEYBOARD
    // keyboard
    get()->emplace_back(
            Option("KEYBOARD", {"KEYBOARD"}, 0, Option::Index::MENU_KEYBOARD, Option::Type::MENU));
    get()->emplace_back(Option("KEY_UP", {std::to_string(KEY_KB_UP_DEFAULT)}, KEY_KB_UP_DEFAULT,
                                           Option::Index::KEY_UP, Option::Type::INPUT));        // KP_UP
    get()->emplace_back(Option("KEY_DOWN", {std::to_string(KEY_KB_DOWN_DEFAULT)}, KEY_KB_DOWN_DEFAULT,
                                           Option::Index::KEY_DOWN, Option::Type::INPUT));    // KP_DOWN
    get()->emplace_back(Option("KEY_LEFT", {std::to_string(KEY_KB_LEFT_DEFAULT)}, KEY_KB_LEFT_DEFAULT,
                                           Option::Index::KEY_LEFT, Option::Type::INPUT));    // KP_LEFT
    get()->emplace_back(Option("KEY_RIGHT", {std::to_string(KEY_KB_RIGHT_DEFAULT)}, KEY_KB_RIGHT_DEFAULT,
                                           Option::Index::KEY_RIGHT, Option::Type::INPUT));  // KP_RIGHT
    get()->emplace_back(Option("KEY_FIRE1", {std::to_string(KEY_KB_FIRE1_DEFAULT)}, KEY_KB_FIRE1_DEFAULT,
                                           Option::Index::KEY_FIRE1, Option::Type::INPUT));  // KP_1
    get()->emplace_back(Option("KEY_FIRE2", {std::to_string(KEY_KB_FIRE2_DEFAULT)}, KEY_KB_FIRE2_DEFAULT,
                                           Option::Index::KEY_FIRE2, Option::Type::INPUT));  // KP_2
    get()->emplace_back(Option("KEY_FIRE3", {std::to_string(KEY_KB_FIRE3_DEFAULT)}, KEY_KB_FIRE3_DEFAULT,
                                           Option::Index::KEY_FIRE3, Option::Type::INPUT));  // KP_3
    get()->emplace_back(Option("KEY_FIRE4", {std::to_string(KEY_KB_FIRE4_DEFAULT)}, KEY_KB_FIRE4_DEFAULT,
                                           Option::Index::KEY_FIRE4, Option::Type::INPUT));  // KP_4
    get()->emplace_back(Option("KEY_FIRE5", {std::to_string(KEY_KB_FIRE5_DEFAULT)}, KEY_KB_FIRE5_DEFAULT,
                                           Option::Index::KEY_FIRE5, Option::Type::INPUT));  // KP_5
    get()->emplace_back(Option("KEY_FIRE6", {std::to_string(KEY_KB_FIRE6_DEFAULT)}, KEY_KB_FIRE6_DEFAULT,
                                           Option::Index::KEY_FIRE6, Option::Type::INPUT));  // KP_6
    get()->emplace_back(Option("KEY_COIN1", {std::to_string(KEY_KB_COIN1_DEFAULT)}, KEY_KB_COIN1_DEFAULT,
                                           Option::Index::KEY_COIN1, Option::Type::INPUT));  // ESCAPE
    get()->emplace_back(Option("KEY_START1", {std::to_string(KEY_KB_START1_DEFAULT)}, KEY_KB_START1_DEFAULT,
                                           Option::Index::KEY_START1, Option::Type::INPUT));// ENTER
#endif

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

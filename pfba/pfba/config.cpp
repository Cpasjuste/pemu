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
        : C2DUIConfig(home, version) {

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
    getHardwareList()->emplace_back(HARDWARE_PREFIX_ALL, "All");
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
    // options needs to be in sync/order with "C2DUIOption::Index"
    ////////////////////////////////////////////////////////////
    // main/gui config
    getOptions()->emplace_back(
            C2DUIOption("MAIN", {"MAIN"}, 0, C2DUIOption::Index::MENU_MAIN, C2DUIOption::Type::MENU));
    getOptions()->emplace_back(C2DUIOption("SHOW_ALL", {"WORKING", "ALL"}, 1, C2DUIOption::Index::GUI_SHOW_ALL));
    getOptions()->emplace_back(C2DUIOption("SHOW_CLONES", {"OFF", "ON"}, 0, C2DUIOption::Index::GUI_SHOW_CLONES));
    getOptions()->emplace_back(C2DUIOption("SHOW_HARDWARE", hardware_names, 0, C2DUIOption::Index::GUI_SHOW_HARDWARE));
#ifdef __PSP2__
    getOptions()->emplace_back(
            C2DUIOption("SHOW_ICONS", {"OFF", "ON"}, 0, C2DUIOption::Index::GUI_SHOW_ICONS, C2DUIOption::Type::HIDDEN));
#else
    getOptions()->emplace_back(C2DUIOption("SHOW_ICONS", {"OFF", "ON"}, 0, C2DUIOption::Index::GUI_SHOW_ICONS));
#endif
    getOptions()->emplace_back(
            C2DUIOption("FULLSCREEN", {"OFF", "ON"}, 1, C2DUIOption::Index::GUI_FULLSCREEN, C2DUIOption::Type::HIDDEN));
#ifdef __SWITCH__
    getOptions()->emplace_back(C2DUIOption("SINGLE_JOYCONS", {"OFF", "ON"}, 0, C2DUIOption::Index::JOY_SINGLEJOYCON));
#endif

    // skin config, hidden in gui for now
    getOptions()->emplace_back(
            C2DUIOption("SKIN", {"SKIN"}, 0, C2DUIOption::Index::MENU_SKIN,
                        C2DUIOption::Type::MENU | C2DUIOption::Type::HIDDEN));
    getOptions()->emplace_back(
            C2DUIOption("SKIN_FONT_SIZE", {"20"}, 20, C2DUIOption::Index::SKIN_FONT_SIZE, C2DUIOption::Type::HIDDEN));
    // default rom config
    getOptions()->emplace_back(
            C2DUIOption("EMULATION", {"EMULATION"}, 0, C2DUIOption::Index::MENU_ROM_OPTIONS, C2DUIOption::Type::MENU));
    getOptions()->emplace_back(
            C2DUIOption("SCALING", {"NONE", "2X", "3X", "FIT", "FIT 4:3", "FULL"},
                        4, C2DUIOption::Index::ROM_SCALING));
    getOptions()->emplace_back(
            C2DUIOption("FILTER", {"POINT", "LINEAR"}, 0, C2DUIOption::Index::ROM_FILTER));
    if (renderer->getShaderList() != nullptr) {
        getOptions()->emplace_back(
                C2DUIOption("EFFECT", renderer->getShaderList()->getNames(), 0, C2DUIOption::Index::ROM_SHADER));
    }
#ifdef __PSP2__
    getOptions()->emplace_back(
            C2DUIOption("ROTATION", {"OFF", "ON", "FLIP", "CAB MODE"}, 0, C2DUIOption::Index::ROM_ROTATION));
#else
    getOptions()->emplace_back(
            C2DUIOption("ROTATION", {"OFF", "ON", "FLIP"}, 0, C2DUIOption::Index::ROM_ROTATION));
#endif
    getOptions()->emplace_back(C2DUIOption("SHOW_FPS", {"OFF", "ON"}, 0, C2DUIOption::Index::ROM_SHOW_FPS));
    getOptions()->emplace_back(C2DUIOption("FRAMESKIP", {"OFF", "1", "2", "3", "4", "5", "6", "7", "8", "9"},
                                           0, C2DUIOption::Index::ROM_FRAMESKIP, C2DUIOption::Type::INTEGER));
    //getOptions()->emplace_back(C2DUIOption("M68K", {"ASM", "C"}, 0, C2DUIOption::Index::ROM_M68K));
    getOptions()->emplace_back(
            C2DUIOption("NEOBIOS", {"UNIBIOS_3_2", "AES_ASIA", "AES_JPN", "DEVKIT", "MVS_ASIA_EUR_V6S1",
                                    "MVS_ASIA_EUR_V5S1", "MVS_ASIA_EUR_V3S4", "MVS_USA_V5S2",
                                    "MVS_USA_V5S4", "MVS_USA_V5S6", "MVS_JPN_V6", "MVS_JPN_V5",
                                    "MVS_JPN_V3S4", "NEO_MVH_MV1C", "MVS_JPN_J3", "DECK_V6"},
                        0, C2DUIOption::Index::ROM_NEOBIOS));
    getOptions()->emplace_back(
            C2DUIOption("AUDIO", {"OFF", "ON"}, 1, C2DUIOption::Index::ROM_AUDIO, C2DUIOption::Type::HIDDEN));

    // joystick
    getOptions()->emplace_back(
            C2DUIOption("JOYPAD", {"JOYPAD"}, 0, C2DUIOption::Index::MENU_JOYPAD, C2DUIOption::Type::MENU));
    getOptions()->emplace_back(
            C2DUIOption("JOY_UP", {"-1"}, KEY_JOY_UP_DEFAULT, C2DUIOption::Index::JOY_UP, C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_DOWN", {"-1"}, KEY_JOY_DOWN_DEFAULT, C2DUIOption::Index::JOY_DOWN,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_LEFT", {"-1"}, KEY_JOY_LEFT_DEFAULT, C2DUIOption::Index::JOY_LEFT,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_RIGHT", {"-1"}, KEY_JOY_RIGHT_DEFAULT, C2DUIOption::Index::JOY_RIGHT,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_FIRE1", {"0"}, KEY_JOY_FIRE1_DEFAULT, C2DUIOption::Index::JOY_FIRE1,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_FIRE2", {"1"}, KEY_JOY_FIRE2_DEFAULT, C2DUIOption::Index::JOY_FIRE2,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_FIRE3", {"2"}, KEY_JOY_FIRE3_DEFAULT, C2DUIOption::Index::JOY_FIRE3,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_FIRE4", {"3"}, KEY_JOY_FIRE4_DEFAULT, C2DUIOption::Index::JOY_FIRE4,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_FIRE5", {"4"}, KEY_JOY_FIRE5_DEFAULT, C2DUIOption::Index::JOY_FIRE5,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_FIRE6", {"5"}, KEY_JOY_FIRE6_DEFAULT, C2DUIOption::Index::JOY_FIRE6,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_COIN1", {"6"}, KEY_JOY_COIN1_DEFAULT, C2DUIOption::Index::JOY_COIN1,
                        C2DUIOption::Type::INPUT));
    getOptions()->emplace_back(
            C2DUIOption("JOY_START1", {"7"}, KEY_JOY_START1_DEFAULT, C2DUIOption::Index::JOY_START1,
                        C2DUIOption::Type::INPUT));
    // TODO: add gui option for axis in option menu
    getOptions()->emplace_back(
            C2DUIOption("JOY_AXIS_LX", {"0"}, KEY_JOY_AXIS_LX, C2DUIOption::Index::JOY_AXIS_LX,
                        C2DUIOption::Type::HIDDEN));
    getOptions()->emplace_back(
            C2DUIOption("JOY_AXIS_LY", {"1"}, KEY_JOY_AXIS_LY, C2DUIOption::Index::JOY_AXIS_LY,
                        C2DUIOption::Type::HIDDEN));
    getOptions()->emplace_back(
            C2DUIOption("JOY_AXIS_RX", {"2"}, KEY_JOY_AXIS_RX, C2DUIOption::Index::JOY_AXIS_RX,
                        C2DUIOption::Type::HIDDEN));
    getOptions()->emplace_back(
            C2DUIOption("JOY_AXIS_RY", {"3"}, KEY_JOY_AXIS_RY, C2DUIOption::Index::JOY_AXIS_RY,
                        C2DUIOption::Type::HIDDEN));
    getOptions()->emplace_back(C2DUIOption("JOY_DEADZONE",
                                           {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
                                            "18000", "20000", "22000", "24000", "26000", "28000", "30000"}, 3,
                                           C2DUIOption::Index::JOY_DEADZONE, C2DUIOption::Type::INTEGER));

#ifndef NO_KEYBOARD
    // keyboard
    getOptions()->emplace_back(
            C2DUIOption("KEYBOARD", {"KEYBOARD"}, 0, C2DUIOption::Index::MENU_KEYBOARD, C2DUIOption::Type::MENU));
    getOptions()->emplace_back(C2DUIOption("KEY_UP", {std::to_string(KEY_KB_UP_DEFAULT)}, KEY_KB_UP_DEFAULT,
                                           C2DUIOption::Index::KEY_UP, C2DUIOption::Type::INPUT));        // KP_UP
    getOptions()->emplace_back(C2DUIOption("KEY_DOWN", {std::to_string(KEY_KB_DOWN_DEFAULT)}, KEY_KB_DOWN_DEFAULT,
                                           C2DUIOption::Index::KEY_DOWN, C2DUIOption::Type::INPUT));    // KP_DOWN
    getOptions()->emplace_back(C2DUIOption("KEY_LEFT", {std::to_string(KEY_KB_LEFT_DEFAULT)}, KEY_KB_LEFT_DEFAULT,
                                           C2DUIOption::Index::KEY_LEFT, C2DUIOption::Type::INPUT));    // KP_LEFT
    getOptions()->emplace_back(C2DUIOption("KEY_RIGHT", {std::to_string(KEY_KB_RIGHT_DEFAULT)}, KEY_KB_RIGHT_DEFAULT,
                                           C2DUIOption::Index::KEY_RIGHT, C2DUIOption::Type::INPUT));  // KP_RIGHT
    getOptions()->emplace_back(C2DUIOption("KEY_FIRE1", {std::to_string(KEY_KB_FIRE1_DEFAULT)}, KEY_KB_FIRE1_DEFAULT,
                                           C2DUIOption::Index::KEY_FIRE1, C2DUIOption::Type::INPUT));  // KP_1
    getOptions()->emplace_back(C2DUIOption("KEY_FIRE2", {std::to_string(KEY_KB_FIRE2_DEFAULT)}, KEY_KB_FIRE2_DEFAULT,
                                           C2DUIOption::Index::KEY_FIRE2, C2DUIOption::Type::INPUT));  // KP_2
    getOptions()->emplace_back(C2DUIOption("KEY_FIRE3", {std::to_string(KEY_KB_FIRE3_DEFAULT)}, KEY_KB_FIRE3_DEFAULT,
                                           C2DUIOption::Index::KEY_FIRE3, C2DUIOption::Type::INPUT));  // KP_3
    getOptions()->emplace_back(C2DUIOption("KEY_FIRE4", {std::to_string(KEY_KB_FIRE4_DEFAULT)}, KEY_KB_FIRE4_DEFAULT,
                                           C2DUIOption::Index::KEY_FIRE4, C2DUIOption::Type::INPUT));  // KP_4
    getOptions()->emplace_back(C2DUIOption("KEY_FIRE5", {std::to_string(KEY_KB_FIRE5_DEFAULT)}, KEY_KB_FIRE5_DEFAULT,
                                           C2DUIOption::Index::KEY_FIRE5, C2DUIOption::Type::INPUT));  // KP_5
    getOptions()->emplace_back(C2DUIOption("KEY_FIRE6", {std::to_string(KEY_KB_FIRE6_DEFAULT)}, KEY_KB_FIRE6_DEFAULT,
                                           C2DUIOption::Index::KEY_FIRE6, C2DUIOption::Type::INPUT));  // KP_6
    getOptions()->emplace_back(C2DUIOption("KEY_COIN1", {std::to_string(KEY_KB_COIN1_DEFAULT)}, KEY_KB_COIN1_DEFAULT,
                                           C2DUIOption::Index::KEY_COIN1, C2DUIOption::Type::INPUT));  // ESCAPE
    getOptions()->emplace_back(C2DUIOption("KEY_START1", {std::to_string(KEY_KB_START1_DEFAULT)}, KEY_KB_START1_DEFAULT,
                                           C2DUIOption::Index::KEY_START1, C2DUIOption::Type::INPUT));// ENTER
#endif

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

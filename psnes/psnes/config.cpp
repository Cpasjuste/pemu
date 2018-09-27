//
// Created by cpasjuste on 29/05/18.
//

#include <string>
#include <vector>

#include "c2dui.h"
#include "config.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

PSNESConfig::PSNESConfig(Renderer *renderer, const std::string &home, int version)
        : C2DUIConfig(home, version) {

    // add hardware list
    getHardwareList()->emplace_back(HARDWARE_PREFIX_ALL, "All");
    std::vector<std::string> hardware_names;
    for (auto &i : *getHardwareList()) {
        hardware_names.emplace_back(i.name);
    }

    std::vector<C2DUIOption> *ui_options = getOptions();

    // main/gui config
    ui_options->emplace_back(
            C2DUIOption("MAIN", {"MAIN"}, 0, C2DUIOption::Index::MENU_MAIN, C2DUIOption::Type::MENU));
    ui_options->emplace_back(C2DUIOption("SHOW_ALL", {"WORKING", "ALL"}, 1, C2DUIOption::Index::GUI_SHOW_ALL));
    ui_options->emplace_back(C2DUIOption("SHOW_CLONES", {"OFF", "ON"}, 0, C2DUIOption::Index::GUI_SHOW_CLONES));
    ui_options->emplace_back(C2DUIOption("SHOW_ICONS", {"OFF", "ON"}, 0, C2DUIOption::Index::GUI_SHOW_ICONS));
    ui_options->emplace_back(C2DUIOption("USE_DATABASE", {"OFF", "ON"}, 0, C2DUIOption::Index::GUI_USE_DATABASE));
    ui_options->emplace_back(
            C2DUIOption("FULLSCREEN", {"OFF", "ON"}, 1, C2DUIOption::Index::GUI_FULLSCREEN, C2DUIOption::Type::HIDDEN));
#ifdef __SWITCH__
    ui_options->emplace_back(C2DUIOption("SINGLE_JOYCONS", {"OFF", "ON"}, 0, C2DUIOption::Index::JOY_SINGLEJOYCON));
#endif

    // skin config, hidden in gui for now
    ui_options->emplace_back(
            C2DUIOption("SKIN", {"SKIN"}, 0, C2DUIOption::Index::MENU_SKIN,
                        C2DUIOption::Type::MENU | C2DUIOption::Type::HIDDEN));
    ui_options->emplace_back(
            C2DUIOption("SKIN_FONT_SIZE", {"20"}, 20, C2DUIOption::Index::SKIN_FONT_SIZE, C2DUIOption::Type::HIDDEN));
    // default rom config
    ui_options->emplace_back(
            C2DUIOption("EMULATION", {"EMULATION"}, 0, C2DUIOption::Index::MENU_ROM_OPTIONS, C2DUIOption::Type::MENU));
    ui_options->emplace_back(
            C2DUIOption("SCALING", {"2X", "3X", "FIT", "FIT 4:3", "FULL"}, 1, C2DUIOption::Index::ROM_SCALING));
    ui_options->emplace_back(
            C2DUIOption("FILTER", {"POINT", "LINEAR"}, 0, C2DUIOption::Index::ROM_FILTER));
    if (renderer->getShaderList() != nullptr) {
        ui_options->emplace_back(
                C2DUIOption("EFFECT", renderer->getShaderList()->getNames(), 0, C2DUIOption::Index::ROM_SHADER));
    } else {
        ui_options->emplace_back(
                C2DUIOption("EFFECT", {"NA"}, 0, C2DUIOption::Index::ROM_SHADER, C2DUIOption::Type::HIDDEN));
    }
    ui_options->emplace_back(
            C2DUIOption("SHOW_FPS", {"OFF", "ON"}, 0, C2DUIOption::Index::ROM_SHOW_FPS));
    ui_options->emplace_back(
            C2DUIOption("HIGH_RES", {"OFF", "ON"}, 0, C2DUIOption::Index::ROM_HIGH_RES));
    ui_options->at(ui_options->size() - 1).setInfo("Only enable high resolution mode for games which can use it,\n"
                                                   "for example \"Secret Of Mana\".\n"
                                                   "It does have some performance impact.\n\n"
                                                   "Need a restart...");
    ui_options->emplace_back(
            C2DUIOption("CHEATS", {"OFF", "ON"}, 1, C2DUIOption::Index::ROM_CHEATS));

    // joystick
    ui_options->emplace_back(
            C2DUIOption("JOYPAD", {"JOYPAD"}, 0, C2DUIOption::Index::MENU_JOYPAD, C2DUIOption::Type::MENU));
    ui_options->emplace_back(
            C2DUIOption("JOY_UP", {"-1"}, KEY_JOY_UP_DEFAULT, C2DUIOption::Index::JOY_UP, C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_DOWN", {"-1"}, KEY_JOY_DOWN_DEFAULT, C2DUIOption::Index::JOY_DOWN,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_LEFT", {"-1"}, KEY_JOY_LEFT_DEFAULT, C2DUIOption::Index::JOY_LEFT,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_RIGHT", {"-1"}, KEY_JOY_RIGHT_DEFAULT, C2DUIOption::Index::JOY_RIGHT,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_FIRE1", {"0"}, KEY_JOY_FIRE1_DEFAULT, C2DUIOption::Index::JOY_FIRE1,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_FIRE2", {"1"}, KEY_JOY_FIRE2_DEFAULT, C2DUIOption::Index::JOY_FIRE2,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_FIRE3", {"2"}, KEY_JOY_FIRE3_DEFAULT, C2DUIOption::Index::JOY_FIRE3,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_FIRE4", {"3"}, KEY_JOY_FIRE4_DEFAULT, C2DUIOption::Index::JOY_FIRE4,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_FIRE5", {"4"}, KEY_JOY_FIRE5_DEFAULT, C2DUIOption::Index::JOY_FIRE5,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_FIRE6", {"5"}, KEY_JOY_FIRE6_DEFAULT, C2DUIOption::Index::JOY_FIRE6,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_COIN1", {"6"}, KEY_JOY_COIN1_DEFAULT, C2DUIOption::Index::JOY_COIN1,
                        C2DUIOption::Type::INPUT));
    ui_options->emplace_back(
            C2DUIOption("JOY_START1", {"7"}, KEY_JOY_START1_DEFAULT, C2DUIOption::Index::JOY_START1,
                        C2DUIOption::Type::INPUT));
    // TODO: add gui option for axis in option menu
    ui_options->emplace_back(
            C2DUIOption("JOY_AXIS_LX", {"0"}, KEY_JOY_AXIS_LX, C2DUIOption::Index::JOY_AXIS_LX,
                        C2DUIOption::Type::HIDDEN));
    ui_options->emplace_back(
            C2DUIOption("JOY_AXIS_LY", {"1"}, KEY_JOY_AXIS_LY, C2DUIOption::Index::JOY_AXIS_LY,
                        C2DUIOption::Type::HIDDEN));
    ui_options->emplace_back(
            C2DUIOption("JOY_AXIS_RX", {"2"}, KEY_JOY_AXIS_RX, C2DUIOption::Index::JOY_AXIS_RX,
                        C2DUIOption::Type::HIDDEN));
    ui_options->emplace_back(
            C2DUIOption("JOY_AXIS_RY", {"3"}, KEY_JOY_AXIS_RY, C2DUIOption::Index::JOY_AXIS_RY,
                        C2DUIOption::Type::HIDDEN));
    ui_options->emplace_back(C2DUIOption("JOY_DEADZONE",
                                         {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
                                          "18000", "20000", "22000", "24000", "26000", "28000", "30000"}, 3,
                                         C2DUIOption::Index::JOY_DEADZONE, C2DUIOption::Type::INTEGER));

#ifndef NO_KEYBOARD
    // keyboard
    ui_options->emplace_back(
            C2DUIOption("KEYBOARD", {"KEYBOARD"}, 0, C2DUIOption::Index::MENU_KEYBOARD, C2DUIOption::Type::MENU));
    ui_options->emplace_back(C2DUIOption("KEY_UP", {std::to_string(KEY_KB_UP_DEFAULT)}, KEY_KB_UP_DEFAULT,
                                         C2DUIOption::Index::KEY_UP, C2DUIOption::Type::INPUT));        // KP_UP
    ui_options->emplace_back(C2DUIOption("KEY_DOWN", {std::to_string(KEY_KB_DOWN_DEFAULT)}, KEY_KB_DOWN_DEFAULT,
                                         C2DUIOption::Index::KEY_DOWN, C2DUIOption::Type::INPUT));    // KP_DOWN
    ui_options->emplace_back(C2DUIOption("KEY_LEFT", {std::to_string(KEY_KB_LEFT_DEFAULT)}, KEY_KB_LEFT_DEFAULT,
                                         C2DUIOption::Index::KEY_LEFT, C2DUIOption::Type::INPUT));    // KP_LEFT
    ui_options->emplace_back(C2DUIOption("KEY_RIGHT", {std::to_string(KEY_KB_RIGHT_DEFAULT)}, KEY_KB_RIGHT_DEFAULT,
                                         C2DUIOption::Index::KEY_RIGHT, C2DUIOption::Type::INPUT));  // KP_RIGHT
    ui_options->emplace_back(C2DUIOption("KEY_FIRE1", {std::to_string(KEY_KB_FIRE1_DEFAULT)}, KEY_KB_FIRE1_DEFAULT,
                                         C2DUIOption::Index::KEY_FIRE1, C2DUIOption::Type::INPUT));  // KP_1
    ui_options->emplace_back(C2DUIOption("KEY_FIRE2", {std::to_string(KEY_KB_FIRE2_DEFAULT)}, KEY_KB_FIRE2_DEFAULT,
                                         C2DUIOption::Index::KEY_FIRE2, C2DUIOption::Type::INPUT));  // KP_2
    ui_options->emplace_back(C2DUIOption("KEY_FIRE3", {std::to_string(KEY_KB_FIRE3_DEFAULT)}, KEY_KB_FIRE3_DEFAULT,
                                         C2DUIOption::Index::KEY_FIRE3, C2DUIOption::Type::INPUT));  // KP_3
    ui_options->emplace_back(C2DUIOption("KEY_FIRE4", {std::to_string(KEY_KB_FIRE4_DEFAULT)}, KEY_KB_FIRE4_DEFAULT,
                                         C2DUIOption::Index::KEY_FIRE4, C2DUIOption::Type::INPUT));  // KP_4
    ui_options->emplace_back(C2DUIOption("KEY_FIRE5", {std::to_string(KEY_KB_FIRE5_DEFAULT)}, KEY_KB_FIRE5_DEFAULT,
                                         C2DUIOption::Index::KEY_FIRE5, C2DUIOption::Type::INPUT));  // KP_5
    ui_options->emplace_back(C2DUIOption("KEY_FIRE6", {std::to_string(KEY_KB_FIRE6_DEFAULT)}, KEY_KB_FIRE6_DEFAULT,
                                         C2DUIOption::Index::KEY_FIRE6, C2DUIOption::Type::INPUT));  // KP_6
    ui_options->emplace_back(C2DUIOption("KEY_COIN1", {std::to_string(KEY_KB_COIN1_DEFAULT)}, KEY_KB_COIN1_DEFAULT,
                                         C2DUIOption::Index::KEY_COIN1, C2DUIOption::Type::INPUT));  // ESCAPE
    ui_options->emplace_back(C2DUIOption("KEY_START1", {std::to_string(KEY_KB_START1_DEFAULT)}, KEY_KB_START1_DEFAULT,
                                         C2DUIOption::Index::KEY_START1, C2DUIOption::Type::INPUT));// ENTER
#endif

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

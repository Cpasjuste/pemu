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
        : Config(home, version) {

    // add hardware list
    getHardwareList()->emplace_back(HARDWARE_PREFIX_ALL, "All");
    std::vector<std::string> hardware_names;
    for (auto &i : *getHardwareList()) {
        hardware_names.emplace_back(i.name);
    }

    std::vector<Option> *ui_options = get();

    // main/gui config
    ui_options->emplace_back(
            Option("MAIN", {"MAIN"}, 0, Option::Index::MENU_MAIN, Option::Type::MENU));
    ui_options->emplace_back(Option("SHOW_ALL", {"WORKING", "ALL"}, 1, Option::Index::GUI_SHOW_ALL));
    ui_options->emplace_back(Option("SHOW_CLONES", {"OFF", "ON"}, 0, Option::Index::GUI_SHOW_CLONES));
    ui_options->emplace_back(Option("SHOW_ICONS", {"OFF", "ON"}, 0, Option::Index::GUI_SHOW_ICONS));
    ui_options->emplace_back(Option("USE_DATABASE", {"OFF", "ON"}, 0, Option::Index::GUI_USE_DATABASE));
    ui_options->emplace_back(
            Option("FULLSCREEN", {"OFF", "ON"}, 1, Option::Index::GUI_FULLSCREEN, Option::Type::HIDDEN));
#ifdef __SWITCH__
    ui_options->emplace_back(Option("SINGLE_JOYCONS", {"OFF", "ON"}, 0, Option::Index::JOY_SINGLEJOYCON));
#endif

    // skin config, hidden in gui for now
    ui_options->emplace_back(
            Option("SKIN", {"SKIN"}, 0, Option::Index::MENU_SKIN,
                        Option::Type::MENU | Option::Type::HIDDEN));
    ui_options->emplace_back(
            Option("SKIN_FONT_SIZE", {"20"}, 20, Option::Index::SKIN_FONT_SIZE, Option::Type::HIDDEN));
    // default rom config
    ui_options->emplace_back(
            Option("EMULATION", {"EMULATION"}, 0, Option::Index::MENU_ROM_OPTIONS, Option::Type::MENU));
    ui_options->emplace_back(
            Option("SCALING", {"2X", "3X", "FIT", "FIT 4:3", "FULL"}, 1, Option::Index::ROM_SCALING));
    ui_options->emplace_back(
            Option("FILTER", {"POINT", "LINEAR"}, 0, Option::Index::ROM_FILTER));
    if (renderer->getShaderList() != nullptr) {
        ui_options->emplace_back(
                Option("EFFECT", renderer->getShaderList()->getNames(), 0, Option::Index::ROM_SHADER));
    } else {
        ui_options->emplace_back(
                Option("EFFECT", {"NA"}, 0, Option::Index::ROM_SHADER, Option::Type::HIDDEN));
    }
    ui_options->emplace_back(
            Option("SHOW_FPS", {"OFF", "ON"}, 0, Option::Index::ROM_SHOW_FPS));
    ui_options->emplace_back(
            Option("HIGH_RES", {"OFF", "ON"}, 0, Option::Index::ROM_HIGH_RES));
    ui_options->at(ui_options->size() - 1).setInfo("Only enable high resolution mode for games which can use it,\n"
                                                   "for example \"Secret Of Mana\".\n"
                                                   "It does have some performance impact.\n\n"
                                                   "Need a restart...");
    ui_options->emplace_back(
            Option("CHEATS", {"OFF", "ON"}, 1, Option::Index::ROM_CHEATS));

    // joystick
    ui_options->emplace_back(
            Option("JOYPAD", {"JOYPAD"}, 0, Option::Index::MENU_JOYPAD, Option::Type::MENU));
    ui_options->emplace_back(
            Option("JOY_UP", {"-1"}, KEY_JOY_UP_DEFAULT, Option::Index::JOY_UP, Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_DOWN", {"-1"}, KEY_JOY_DOWN_DEFAULT, Option::Index::JOY_DOWN,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_LEFT", {"-1"}, KEY_JOY_LEFT_DEFAULT, Option::Index::JOY_LEFT,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_RIGHT", {"-1"}, KEY_JOY_RIGHT_DEFAULT, Option::Index::JOY_RIGHT,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_FIRE1", {"0"}, KEY_JOY_FIRE1_DEFAULT, Option::Index::JOY_FIRE1,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_FIRE2", {"1"}, KEY_JOY_FIRE2_DEFAULT, Option::Index::JOY_FIRE2,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_FIRE3", {"2"}, KEY_JOY_FIRE3_DEFAULT, Option::Index::JOY_FIRE3,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_FIRE4", {"3"}, KEY_JOY_FIRE4_DEFAULT, Option::Index::JOY_FIRE4,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_FIRE5", {"4"}, KEY_JOY_FIRE5_DEFAULT, Option::Index::JOY_FIRE5,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_FIRE6", {"5"}, KEY_JOY_FIRE6_DEFAULT, Option::Index::JOY_FIRE6,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_COIN1", {"6"}, KEY_JOY_COIN1_DEFAULT, Option::Index::JOY_COIN1,
                        Option::Type::INPUT));
    ui_options->emplace_back(
            Option("JOY_START1", {"7"}, KEY_JOY_START1_DEFAULT, Option::Index::JOY_START1,
                        Option::Type::INPUT));
    // TODO: add gui option for axis in option menu
    ui_options->emplace_back(
            Option("JOY_AXIS_LX", {"0"}, KEY_JOY_AXIS_LX, Option::Index::JOY_AXIS_LX,
                        Option::Type::HIDDEN));
    ui_options->emplace_back(
            Option("JOY_AXIS_LY", {"1"}, KEY_JOY_AXIS_LY, Option::Index::JOY_AXIS_LY,
                        Option::Type::HIDDEN));
    ui_options->emplace_back(
            Option("JOY_AXIS_RX", {"2"}, KEY_JOY_AXIS_RX, Option::Index::JOY_AXIS_RX,
                        Option::Type::HIDDEN));
    ui_options->emplace_back(
            Option("JOY_AXIS_RY", {"3"}, KEY_JOY_AXIS_RY, Option::Index::JOY_AXIS_RY,
                        Option::Type::HIDDEN));
    ui_options->emplace_back(Option("JOY_DEADZONE",
                                         {"2000", "4000", "6000", "8000", "10000", "12000", "14000", "16000",
                                          "18000", "20000", "22000", "24000", "26000", "28000", "30000"}, 3,
                                         Option::Index::JOY_DEADZONE, Option::Type::INTEGER));

#ifndef NO_KEYBOARD
    // keyboard
    ui_options->emplace_back(
            Option("KEYBOARD", {"KEYBOARD"}, 0, Option::Index::MENU_KEYBOARD, Option::Type::MENU));
    ui_options->emplace_back(Option("KEY_UP", {std::to_string(KEY_KB_UP_DEFAULT)}, KEY_KB_UP_DEFAULT,
                                         Option::Index::KEY_UP, Option::Type::INPUT));        // KP_UP
    ui_options->emplace_back(Option("KEY_DOWN", {std::to_string(KEY_KB_DOWN_DEFAULT)}, KEY_KB_DOWN_DEFAULT,
                                         Option::Index::KEY_DOWN, Option::Type::INPUT));    // KP_DOWN
    ui_options->emplace_back(Option("KEY_LEFT", {std::to_string(KEY_KB_LEFT_DEFAULT)}, KEY_KB_LEFT_DEFAULT,
                                         Option::Index::KEY_LEFT, Option::Type::INPUT));    // KP_LEFT
    ui_options->emplace_back(Option("KEY_RIGHT", {std::to_string(KEY_KB_RIGHT_DEFAULT)}, KEY_KB_RIGHT_DEFAULT,
                                         Option::Index::KEY_RIGHT, Option::Type::INPUT));  // KP_RIGHT
    ui_options->emplace_back(Option("KEY_FIRE1", {std::to_string(KEY_KB_FIRE1_DEFAULT)}, KEY_KB_FIRE1_DEFAULT,
                                         Option::Index::KEY_FIRE1, Option::Type::INPUT));  // KP_1
    ui_options->emplace_back(Option("KEY_FIRE2", {std::to_string(KEY_KB_FIRE2_DEFAULT)}, KEY_KB_FIRE2_DEFAULT,
                                         Option::Index::KEY_FIRE2, Option::Type::INPUT));  // KP_2
    ui_options->emplace_back(Option("KEY_FIRE3", {std::to_string(KEY_KB_FIRE3_DEFAULT)}, KEY_KB_FIRE3_DEFAULT,
                                         Option::Index::KEY_FIRE3, Option::Type::INPUT));  // KP_3
    ui_options->emplace_back(Option("KEY_FIRE4", {std::to_string(KEY_KB_FIRE4_DEFAULT)}, KEY_KB_FIRE4_DEFAULT,
                                         Option::Index::KEY_FIRE4, Option::Type::INPUT));  // KP_4
    ui_options->emplace_back(Option("KEY_FIRE5", {std::to_string(KEY_KB_FIRE5_DEFAULT)}, KEY_KB_FIRE5_DEFAULT,
                                         Option::Index::KEY_FIRE5, Option::Type::INPUT));  // KP_5
    ui_options->emplace_back(Option("KEY_FIRE6", {std::to_string(KEY_KB_FIRE6_DEFAULT)}, KEY_KB_FIRE6_DEFAULT,
                                         Option::Index::KEY_FIRE6, Option::Type::INPUT));  // KP_6
    ui_options->emplace_back(Option("KEY_COIN1", {std::to_string(KEY_KB_COIN1_DEFAULT)}, KEY_KB_COIN1_DEFAULT,
                                         Option::Index::KEY_COIN1, Option::Type::INPUT));  // ESCAPE
    ui_options->emplace_back(Option("KEY_START1", {std::to_string(KEY_KB_START1_DEFAULT)}, KEY_KB_START1_DEFAULT,
                                         Option::Index::KEY_START1, Option::Type::INPUT));// ENTER
#endif

    // set default rom options
    reset();

    // load/overwrite configuration from file
    load();
}

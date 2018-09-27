/*
 * FinalBurn Alpha
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <sys/stat.h>

#include "c2dui.h"
#include "uiEmu.h"
#include "uiMenu.h"
#include "config.h"
#include "romlist.h"

#include "snes9x.h"

using namespace c2d;
using namespace c2dui;

#ifdef __PSP2__

#include <psp2/power.h>
#include <psp2/io/dirent.h>

#define mkdir(x, y) sceIoMkdir(x, 0777)
int _newlib_heap_size_user = 192 * 1024 * 1024;
#define SCR_W   960
#define SCR_H   544
#elif __PS3__
#define SCR_W   1280
#define SCR_H   720
#elif __3DS__
#define SCR_W   400
#define SCR_H   240
#elif __SWITCH__
#define SCR_W   1280
#define SCR_H   720
#else
#define SCR_W   1280
#define SCR_H   720
#endif

Renderer *renderer;
Input *inp;
Io *io;

PSNESGuiMenu *uiMenu;
PSNESGuiEmu *uiEmu;
PSNESConfig *config;
PSNESRomList *romList;

C2DUIGuiMain *ui;
C2DUISkin *skin;
C2DUIGuiRomList *uiRomList;
C2DUIGuiState *uiState;

int main(int argc, char **argv) {

    // create renderer
    renderer = new C2DRenderer(Vector2f(SCR_W, SCR_H));
#ifndef __PSP2__
#ifndef __GL__
    renderer->setShaderList(new ShaderList());
    renderer->getShaderList()->add("TV2X", nullptr);
    renderer->getShaderList()->add("SMOOTH", nullptr);
    renderer->getShaderList()->add("SUPEREAGLE", nullptr);
    renderer->getShaderList()->add("2XSAI", nullptr);
    renderer->getShaderList()->add("SUPER2XSAI", nullptr);
    renderer->getShaderList()->add("EPX", nullptr);
    renderer->getShaderList()->add("HQ2X", nullptr);
#endif
#endif

    // create inputs, io
    inp = new C2DInput();
    io = new C2DIo();

    // load configuration
    int psnes_version = (__PSNES_VERSION_MAJOR__ * 100) + __PSNES_VERSION_MINOR__;
    config = new PSNESConfig(renderer, C2DUI_HOME_PATH, psnes_version);
    std::string configs_path = *config->getHomePath() + "configs";
    mkdir(configs_path.c_str(), 0755);

    // buttons used for ui config menu
    std::vector<C2DUISkin::Button> buttons;
#ifdef __PSP2__
    // set max cpu speed
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
    // see c2d.h for key id
    buttons.emplace_back(KEY_JOY_UP_DEFAULT, "UP");
    buttons.emplace_back(KEY_JOY_DOWN_DEFAULT, "DOWN");
    buttons.emplace_back(KEY_JOY_LEFT_DEFAULT, "LEFT");
    buttons.emplace_back(KEY_JOY_RIGHT_DEFAULT, "RIGHT");
    buttons.emplace_back(KEY_JOY_FIRE1_DEFAULT, "TRIANGLE");
    buttons.emplace_back(KEY_JOY_FIRE2_DEFAULT, "CIRCLE");
    buttons.emplace_back(KEY_JOY_FIRE3_DEFAULT, "CROSS");
    buttons.emplace_back(KEY_JOY_FIRE4_DEFAULT, "SQUARE");
    buttons.emplace_back(KEY_JOY_FIRE5_DEFAULT, "L");
    buttons.emplace_back(KEY_JOY_FIRE6_DEFAULT, "R");
    buttons.emplace_back(KEY_JOY_COIN1_DEFAULT, "SELECT");
    buttons.emplace_back(KEY_JOY_START1_DEFAULT, "START");
#elif __SWITCH__
    // see c2d.h for key id
    buttons.emplace_back(KEY_JOY_UP_DEFAULT, "UP");
    buttons.emplace_back(KEY_JOY_DOWN_DEFAULT, "DOWN");
    buttons.emplace_back(KEY_JOY_LEFT_DEFAULT, "LEFT");
    buttons.emplace_back(KEY_JOY_RIGHT_DEFAULT, "RIGHT");
    buttons.emplace_back(KEY_JOY_FIRE1_DEFAULT, "A");
    buttons.emplace_back(KEY_JOY_FIRE2_DEFAULT, "B");
    buttons.emplace_back(KEY_JOY_FIRE3_DEFAULT, "X");
    buttons.emplace_back(KEY_JOY_FIRE4_DEFAULT, "Y");
    buttons.emplace_back(KEY_JOY_FIRE5_DEFAULT, "L");
    buttons.emplace_back(KEY_JOY_FIRE6_DEFAULT, "R");
    buttons.emplace_back(KEY_JOY_COIN1_DEFAULT, "-");
    buttons.emplace_back(KEY_JOY_START1_DEFAULT, "+");
    // switch special keys
    buttons.emplace_back(KEY_JOY_ZL_DEFAULT, "ZL");
    buttons.emplace_back(KEY_JOY_ZR_DEFAULT, "ZR");
    buttons.emplace_back(KEY_JOY_LSTICK_DEFAULT, "LSTICK");
    buttons.emplace_back(KEY_JOY_RSTICK_DEFAULT, "RSTICK");
#endif
    // skin
#ifdef __PSP2__
    skin = new C2DUISkin("app0:/", buttons);
#else
    skin = new C2DUISkin(C2DUI_HOME_PATH, buttons);
#endif

    // gui
    ui = new C2DUIGuiMain(renderer, io, inp, config, skin);
    // build rom list
    std::string snes9x_version = "snes9x ";
    snes9x_version += VERSION;
    romList = new PSNESRomList(ui, snes9x_version);
    romList->build();
    // rom list ui
    uiRomList = new C2DUIGuiRomList(ui, romList, renderer->getSize());
    // menu ui
    uiMenu = new PSNESGuiMenu(ui);
    // in game emu ui
    uiEmu = new PSNESGuiEmu(ui);
    // states menu ui
    uiState = new C2DUIGuiState(ui);
    // run that crap
    ui->init(uiRomList, uiMenu, uiEmu, uiState);
    ui->run();

    // cleanup
    delete (ui);
    delete (skin);
    delete (config);
    delete (io);
    delete (inp);
    delete (renderer);

#ifdef __PSP2__
    scePowerSetArmClockFrequency(266);
    scePowerSetBusClockFrequency(166);
    scePowerSetGpuClockFrequency(166);
    scePowerSetGpuXbarClockFrequency(111);
#endif

    return 0;
}

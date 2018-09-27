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

#include "burner.h"

#include "c2dui.h"
#include "ui.h"
#include "uiEmu.h"
#include "uiMenu.h"
#include "config.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

#ifdef __PSP2__

#include <psp2/power.h>
#include <psp2/io/dirent.h>

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

PFBAGui *ui;
PFBAGuiMenu *uiMenu;
PFBAGuiEmu *uiEmu;
PFBAConfig *config;
PFBARomList *romList;

C2DUISkin *skin;
C2DUIGuiRomList *uiRomList;
C2DUIGuiState *uiState;

int main(int argc, char **argv) {

    BurnPathsInit();
    BurnLibInit();

    renderer = new C2DRenderer(Vector2f(SCR_W, SCR_H));
    inp = new C2DInput();
    io = new C2DIo();

    // load configuration
    int version = (__PFBA_VERSION_MAJOR__ * 100) + __PFBA_VERSION_MINOR__;
    config = new PFBAConfig(renderer, C2DUI_HOME_PATH, version);

    // skin
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
    skin = new C2DUISkin("app0:/", buttons);
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
    skin = new C2DUISkin(C2DUI_HOME_PATH, buttons);
#else
    skin = new C2DUISkin(C2DUI_HOME_PATH, buttons);
#endif

    // gui
    ui = new PFBAGui(renderer, io, inp, config, skin);
    std::string fba_version = "fba: ";
    fba_version += szAppBurnVer;
    romList = new PFBARomList(ui, fba_version);
    romList->build();
    uiRomList = new C2DUIGuiRomList(ui, romList, renderer->getSize());
    uiMenu = new PFBAGuiMenu(ui);
    uiEmu = new PFBAGuiEmu(ui);
    uiState = new C2DUIGuiState(ui);
    ui->init(uiRomList, uiMenu, uiEmu, uiState);
    ui->run();

    // quit
    BurnLibExit();

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

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

#include "c2dui.h"
#include "pgen_ui_emu.h"
#include "pgen_io.h"
#include "pgen_config.h"
#include "pgen_romlist.h"

using namespace c2d;
using namespace c2dui;

#ifdef __PSP2__
#include <psp2/power.h>
#include <psp2/io/dirent.h>
#define mkdir(x, y) sceIoMkdir(x, 0777)
#elif __PS4__
extern "C" int sceSystemServiceLoadExec(const char *path, const char *args[]);
#endif

UiMenu *uiMenu;
PGENUiEmu *uiEmu;
PGENConfig *cfg;
PGENRomList *romList;
UiStateMenu *uiState;

UiMain *ui;
Skin *skin;
UIRomList *uiRomList;

int main(int argc, char **argv) {
    // need custom io for some devices
    auto *io = new PGENIo();
    // load configuration
    int pgen_version = (__PGEN_VERSION_MAJOR__ * 100) + __PGEN_VERSION_MINOR__;
    cfg = new PGENConfig(io, pgen_version);

    // create paths
    io->create(io->getDataPath());
    io->create(io->getDataPath() + "bios");
    io->create(io->getDataPath() + "roms");
    io->create(io->getDataPath() + "configs");

    Vector2f screenSize = cfg->getScreenSize();
    ui = new UiMain(screenSize, io, cfg);

    // skin buttons used for ui config menu
    std::vector<Skin::Button> buttons;
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
    buttons.emplace_back(KEY_JOY_A_DEFAULT, "CROSS");
    buttons.emplace_back(KEY_JOY_B_DEFAULT, "CIRCLE");
    buttons.emplace_back(KEY_JOY_X_DEFAULT, "SQUARE");
    buttons.emplace_back(KEY_JOY_Y_DEFAULT, "TRIANGLE");
    buttons.emplace_back(KEY_JOY_LT_DEFAULT, "L");
    buttons.emplace_back(KEY_JOY_RT_DEFAULT, "R");
    buttons.emplace_back(KEY_JOY_LB_DEFAULT, "NONE");
    buttons.emplace_back(KEY_JOY_RB_DEFAULT, "NONE");
    buttons.emplace_back(KEY_JOY_SELECT_DEFAULT, "SELECT");
    buttons.emplace_back(KEY_JOY_START_DEFAULT, "START");
    buttons.emplace_back(KEY_JOY_MENU1_DEFAULT, "START");
    buttons.emplace_back(KEY_JOY_MENU2_DEFAULT, "SELECT");
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
    buttons.emplace_back(KEY_JOY_FIRE5_DEFAULT, "ZL");
    buttons.emplace_back(KEY_JOY_FIRE6_DEFAULT, "ZR");
    buttons.emplace_back(KEY_JOY_FIRE7_DEFAULT, "L");
    buttons.emplace_back(KEY_JOY_FIRE8_DEFAULT, "R");
    buttons.emplace_back(KEY_JOY_COIN1_DEFAULT, "-");
    buttons.emplace_back(KEY_JOY_START1_DEFAULT, "+");
    buttons.emplace_back(KEY_JOY_MENU1_DEFAULT, "R");
    buttons.emplace_back(KEY_JOY_MENU2_DEFAULT, "L");
    // switch special keys
    buttons.emplace_back(KEY_JOY_LSTICK_DEFAULT, "LSTICK");
    buttons.emplace_back(KEY_JOY_RSTICK_DEFAULT, "RSTICK");
#elif __PS4__
    // see c2d.h for key id
    buttons.emplace_back(KEY_JOY_UP_DEFAULT, "UP");
    buttons.emplace_back(KEY_JOY_DOWN_DEFAULT, "DOWN");
    buttons.emplace_back(KEY_JOY_LEFT_DEFAULT, "LEFT");
    buttons.emplace_back(KEY_JOY_RIGHT_DEFAULT, "RIGHT");
    buttons.emplace_back(KEY_JOY_FIRE1_DEFAULT, "CROSS");
    buttons.emplace_back(KEY_JOY_FIRE2_DEFAULT, "CIRCLE");
    buttons.emplace_back(KEY_JOY_FIRE3_DEFAULT, "SQUARE");
    buttons.emplace_back(KEY_JOY_FIRE4_DEFAULT, "TRIANGLE");
    buttons.emplace_back(KEY_JOY_FIRE5_DEFAULT, "L2");
    buttons.emplace_back(KEY_JOY_FIRE6_DEFAULT, "R2");
    buttons.emplace_back(KEY_JOY_FIRE7_DEFAULT, "L2");
    buttons.emplace_back(KEY_JOY_FIRE8_DEFAULT, "R2");
    buttons.emplace_back(KEY_JOY_COIN1_DEFAULT, "L1");
    buttons.emplace_back(KEY_JOY_START1_DEFAULT, "R1");
    buttons.emplace_back(KEY_JOY_MENU1_DEFAULT, "L1");
    buttons.emplace_back(KEY_JOY_MENU2_DEFAULT, "R1");
#else
    // see c2d.h for key id
    buttons.emplace_back(KEY_JOY_UP_DEFAULT, "UP");
    buttons.emplace_back(KEY_JOY_DOWN_DEFAULT, "DOWN");
    buttons.emplace_back(KEY_JOY_LEFT_DEFAULT, "LEFT");
    buttons.emplace_back(KEY_JOY_RIGHT_DEFAULT, "RIGHT");
    buttons.emplace_back(KEY_JOY_A_DEFAULT, "CROSS");
    buttons.emplace_back(KEY_JOY_B_DEFAULT, "CIRCLE");
    buttons.emplace_back(KEY_JOY_X_DEFAULT, "SQUARE");
    buttons.emplace_back(KEY_JOY_Y_DEFAULT, "TRIANGLE");
    buttons.emplace_back(KEY_JOY_LT_DEFAULT, "LT");
    buttons.emplace_back(KEY_JOY_RT_DEFAULT, "RT");
    buttons.emplace_back(KEY_JOY_LB_DEFAULT, "LB");
    buttons.emplace_back(KEY_JOY_RB_DEFAULT, "RB");
    buttons.emplace_back(KEY_JOY_LS_DEFAULT, "LS");
    buttons.emplace_back(KEY_JOY_RS_DEFAULT, "RS");
    buttons.emplace_back(KEY_JOY_SELECT_DEFAULT, "SELECT");
    buttons.emplace_back(KEY_JOY_START_DEFAULT, "START");
    buttons.emplace_back(KEY_JOY_MENU1_DEFAULT, "LB");
    buttons.emplace_back(KEY_JOY_MENU2_DEFAULT, "RB");
#endif

    skin = new Skin(ui, buttons);
    ui->setSkin(skin);

    // ui
    std::string genesis_version = "Genesis Plus GX";
    romList = new PGENRomList(ui, genesis_version,
                              {".zip", ".md", ".smd", ".gen", ".bin", ".mdx",
                               ".cue", ".iso", ".sms", ".gg", ".sg", ".68k", ".chd"});
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new UiMenu(ui);
    uiEmu = new PGENUiEmu(ui);
    uiState = new UiStateMenu(ui);
    ui->init(uiRomList, uiMenu, uiEmu, uiState);

    while (!ui->done) {
        ui->flip();
    }

    delete (skin);
    delete (cfg);
    delete (ui);

#ifdef __PSP2__
    scePowerSetArmClockFrequency(266);
    scePowerSetBusClockFrequency(166);
    scePowerSetGpuClockFrequency(166);
    scePowerSetGpuXbarClockFrequency(111);
#elif __PS4__
    sceSystemServiceLoadExec((char *) "exit", nullptr);
    while (true) {}
#endif

    return 0;
}

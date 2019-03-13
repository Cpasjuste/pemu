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
#include "uiStateMenu.h"
#include "config.h"
#include "romlist.h"

#include "snes9x.h"

using namespace c2d;
using namespace c2dui;

#ifdef __PSP2__
#include <psp2/power.h>
#include <psp2/io/dirent.h>
#define C2DUI_HOME_PATH "ux0:/data/psnes/"
#define mkdir(x, y) sceIoMkdir(x, 0777)
int _newlib_heap_size_user = 192 * 1024 * 1024;
#endif

Renderer *renderer;

PSNESUIMenu *uiMenu;
PSNESUIEmu *uiEmu;
PSNESConfig *cfg;
PSNESRomList *romList;
PSNESUIStateMenu *uiState;

UIMain *ui;
Skin *skin;
UIRomList *uiRomList;

int main(int argc, char **argv) {

    // create renderer
    renderer = new C2DRenderer(Vector2f(C2D_SCREEN_WIDTH, C2D_SCREEN_HEIGHT));
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

    // load configuration
    int psnes_version = (__PSNES_VERSION_MAJOR__ * 100) + __PSNES_VERSION_MINOR__;
    cfg = new PSNESConfig(ui->getIo()->getDataWritePath(), psnes_version);
    std::string configs_path = *cfg->getHomePath() + "configs";
    mkdir(configs_path.c_str(), 0755);

    // buttons used for ui config menu
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
    skin = new Skin("app0:/", buttons);
#else
    skin = new Skin(ui, buttons);
#endif
    ui->setSkin(skin);

    // ui
    std::string snes9x_version = "snes9x ";
    snes9x_version += VERSION;
    romList = new PSNESRomList(ui, snes9x_version);
    romList->build();
    uiRomList = new UIRomListClassic(ui, romList, ui->getSize());
    uiMenu = new PSNESUIMenu(ui);
    uiEmu = new PSNESUIEmu(ui);
    uiState = new PSNESUIStateMenu(ui);
    ui->init(uiRomList, uiMenu, uiEmu, uiState);

    while (!ui->done) {
        ui->flip();
    }

    // cleanup
    delete (ui);
    delete (skin);
    delete (cfg);

#ifdef __PSP2__
    scePowerSetArmClockFrequency(266);
    scePowerSetBusClockFrequency(166);
    scePowerSetGpuClockFrequency(166);
    scePowerSetGpuXbarClockFrequency(111);
#endif

    return 0;
}

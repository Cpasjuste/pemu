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
#include "uiEmu.h"
#include "uiMenu.h"
#include "uiStateMenu.h"
#include "pnes_config.h"

using namespace c2d;
using namespace c2dui;

#ifdef __PSP2__
#include <psp2/power.h>
#include <psp2/io/dirent.h>
#define mkdir(x, y) sceIoMkdir(x, 0777)
int _newlib_heap_size_user = 192 * 1024 * 1024;
#endif

PNESGuiMenu *uiMenu;
PNESGuiEmu *uiEmu;
PNESConfig *cfg;
PNESUIStateMenu *uiState;
RomList *romList;

UIMain *ui;
Skin *skin;
UIRomList *uiRomList;

int main(int argc, char **argv) {

    auto *io = new C2DIo();
    // load configuration
    int pnes_version = (__PNES_VERSION_MAJOR__ * 100) + __PNES_VERSION_MINOR__;
    cfg = new PNESConfig(io, pnes_version);

    // create paths
    io->create(cfg->getHomePath() + "configs");
    io->create(cfg->getHomePath() + "saves");
    io->create(cfg->getHomePath() + "cache");
    io->create(cfg->getHomePath() + "mixes");

    Vector2f screen_size = {
            cfg->get(Option::Id::GUI_SCREEN_WIDTH)->getValueInt(),
            cfg->get(Option::Id::GUI_SCREEN_HEIGHT)->getValueInt()
    };

    // we need to create a renderer with real screen size
#ifdef __FULLSCREEN__
    if (cfg->get(Option::Id::GUI_FULLSCREEN)->getValueBool()) {
        screen_size = Vector2f();
    }
#endif
    ui = new UIMain(screen_size);
    if (ui->getShaderList() != nullptr) {
        cfg->add(Option::Id::ROM_FILTER, "EFFECT", ui->getShaderList()->getNames(), 0,
                 Option::Id::ROM_SHADER, Option::Flags::STRING);
    } else {
        cfg->add(Option::Id::ROM_FILTER, "EFFECT", {"NONE"}, 0,
                 Option::Id::ROM_SHADER, Option::Flags::STRING | Option::Flags::HIDDEN);
    }

    ui->setIo(io);
    ui->setConfig(cfg);
#ifndef __FULLSCREEN__
    // now set window size, usefull when screen is "cropped" (freeplay zero/cm3)
    FloatRect windows_size = {
            cfg->get(Option::Id::GUI_WINDOW_LEFT)->getValueInt(),
            cfg->get(Option::Id::GUI_WINDOW_TOP)->getValueInt(),
            cfg->get(Option::Id::GUI_WINDOW_WIDTH)->getValueInt(),
            cfg->get(Option::Id::GUI_WINDOW_HEIGHT)->getValueInt()
    };
    ui->setSize(windows_size.width, windows_size.height);
    ui->setPosition(windows_size.left, windows_size.top);
#endif

    // skin
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
    skin = new Skin(ui, buttons);
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
    skin = new Skin(ui, buttons);
#else
#if __FULLSCREEN__
    int x = 0, y = 0;
    SDL_GetWindowSize(ui->getWindow(), &x, &y);
    Vector2f scale = {(float) x / (float) C2D_SCREEN_WIDTH,
                      (float) y / (float) C2D_SCREEN_HEIGHT};
    skin = new Skin(ui, buttons, scale);
#else
    skin = new Skin(ui, buttons);
#endif
#endif
    ui->setSkin(skin);

    // ui
    std::string nestopia_version = "Nestopia 1.50";
    romList = new RomList(ui, nestopia_version);
    romList->build();
    uiRomList = new UIRomListClassic(ui, romList, ui->getSize());
    uiMenu = new PNESGuiMenu(ui);
    uiEmu = new PNESGuiEmu(ui);
    uiState = new PNESUIStateMenu(ui);
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
#endif

    return 0;
}

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
#include "pgen_ui_statemenu.h"

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
PGENUIStateMenu *uiState;

UiMain *ui;
Skin *skin;
UIRomList *uiRomList;

int main(int argc, char **argv) {
#ifdef __PSP2__
    // set max cpu speed
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
#endif

    // need custom io for some devices
    auto *io = new PGENIo();
    // load configuration
    int pgen_version = (__PGEN_VERSION_MAJOR__ * 100) + __PGEN_VERSION_MINOR__;
    cfg = new PGENConfig(io, pgen_version);

    // create paths
    io->create(io->getDataPath());
    io->create(io->getDataPath() + "bios");
    io->create(io->getDataPath() + "rams");
    io->create(io->getDataPath() + "configs");
    io->create(io->getDataPath() + "saves");
    io->create(io->getDataPath() + "megadrive");
    io->create(io->getDataPath() + "sms");
    io->create(io->getDataPath() + "gamegear");
    io->create(io->getDataPath() + "megacd");

    Vector2f screenSize = cfg->getScreenSize();
    ui = new UiMain(screenSize, io, cfg);

    // skin
    skin = new Skin(ui);
    ui->setSkin(skin);

    // ui
    std::string genesis_version = "Genesis Plus GX";
    romList = new PGENRomList(ui, genesis_version,
                              {".zip", ".md", ".smd", ".gen", ".bin",
                               ".mdx", ".sms", ".gg", ".sg", ".68k", ".chd"});
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new UiMenu(ui);
    uiEmu = new PGENUiEmu(ui);
    uiState = new PGENUIStateMenu(ui);
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

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

#include "pemu.h"
#include "pnes_ui_emu.h"
#include "pnes_ui_menu_state.h"
#include "pnes_config.h"
#include "pnes_io.h"

using namespace c2d;
using namespace pemu;

#ifdef __PSP2__
#include <psp2/power.h>
#include <psp2/io/dirent.h>
#define mkdir(x, y) sceIoMkdir(x, 0777)
#elif __PS4__
extern "C" int sceSystemServiceLoadExec(const char *path, const char *args[]);
#endif

UiMenu *uiMenu;
PNESUiEmu *uiEmu;
PNESConfig *cfg;
PNESUIStateMenu *uiState;
RomList *romList;

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
    auto *io = new PNESIo();
    // create paths
    io->create(io->getDataPath());
    io->create(io->getDataPath() + "roms");
    io->create(io->getDataPath() + "configs");
    io->create(io->getDataPath() + "saves");

    // load custom configuration
    int pnes_version = (__PNES_VERSION_MAJOR__ * 100) + __PNES_VERSION_MINOR__;
    cfg = new PNESConfig(io, pnes_version);

    // create main ui
    c2d::config::Option *fs = cfg->get(PEMUConfig::Id::GUI_FULLSCREEN);
    if (fs && !fs->getInteger()) {
        ui = new UiMain(io, {1280, 720});
    } else {
        ui = new UiMain(io);
    }
    ui->setConfig(cfg);

    // load skin configuration
    skin = new Skin(ui);
    ui->setSkin(skin);

    // ui
    std::string nestopia_version = "Nestopia 1.52.0";
    romList = new RomList(ui, nestopia_version, {".zip", ".nes", ".nez", ".unf", ".unif"});
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new UiMenu(ui);
    uiEmu = new PNESUiEmu(ui);
    uiState = new PNESUIStateMenu(ui);
    ui->init(uiRomList, uiMenu, uiEmu, uiState);

    // save default config
    cfg->save();

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

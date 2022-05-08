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
#include "uiEmu.h"
#include "uiMenu.h"
#include "config.h"
#include "uiStateMenu.h"
#include "pfba_io.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

#ifdef __PSP2__

#include <psp2/power.h>

#elif __PS4__
extern "C" int sceSystemServiceLoadExec(const char *path, const char *args[]);
#endif

UiMain *ui;
PFBAGuiMenu *uiMenu;
PFBAUiEmu *uiEmu;
PFBAUIStateMenu *uiState;
PFBAConfig *cfg;
PFBARomList *romList;
UIRomList *uiRomList;
Skin *skin;

void BurnPathsInit(C2DIo *io);

INT32 GetIpsesMaxLen(char *)
{
	return 0;
}

int main(int argc, char **argv) {
#ifdef __PSP2__
    // set max cpu speed
    scePowerSetArmClockFrequency(444);
    scePowerSetBusClockFrequency(222);
    scePowerSetGpuClockFrequency(222);
    scePowerSetGpuXbarClockFrequency(166);
#endif

    // need custom io for some devices
    auto io = new PFBAIo();
    // load configuration
    int version = (__PFBA_VERSION_MAJOR__ * 100) + __PFBA_VERSION_MINOR__;
    cfg = new PFBAConfig(io, version);

    // fbneo init
    BurnPathsInit(io);
    BurnLibInit();

    Vector2f screenSize = cfg->getScreenSize();
    ui = new UiMain(screenSize, io, cfg);

    // skin
    skin = new Skin(ui);
    ui->setSkin(skin);

    // ui
    std::string fba_version = "fbneo: ";
    fba_version += szAppBurnVer;
    romList = new PFBARomList(ui, fba_version, {".zip"});
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new PFBAGuiMenu(ui);
    uiEmu = new PFBAUiEmu(ui);
    uiState = new PFBAUIStateMenu(ui);
    ui->init(uiRomList, uiMenu, uiEmu, uiState);

    while (!ui->done) {
        ui->flip();
    }

    // quit
    BurnLibExit();

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

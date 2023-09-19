//
// Created by cpasjuste on 22/11/16.
//

#include "pemu.h"
#include "pgen_ui_emu.h"
#include "pgen_io.h"
#include "pgen_config.h"
#include "pgen_romlist.h"
#include "pgen_ui_menu_state.h"
#include "osd.h"

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

    // load custom configuration
    int pgen_version = (__PGEN_VERSION_MAJOR__ * 100) + __PGEN_VERSION_MINOR__;
    cfg = new PGENConfig(io, pgen_version);

    // create main ui/renderer
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
    std::string genesis_version = VERSION;
    romList = new PGENRomList(ui, genesis_version,
                              {".zip", ".md", ".smd", ".gen", ".bin",
                               ".mdx", ".sms", ".gg", ".sg", ".68k"});
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new UiMenu(ui);
    uiEmu = new PGENUiEmu(ui);
    uiState = new PGENUIStateMenu(ui);
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

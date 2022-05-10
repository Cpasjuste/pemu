#include "c2dui.h"
#include "uiEmu.h"
#include "uiStateMenu.h"
#include "config.h"
#include "psnesIo.h"
#include "snes9x.h"

#ifdef __PSP2__
#include <psp2/power.h>
#elif __3DS__
//unsigned int __stacksize__ = 1024 * 1024;
//u32 __ctru_heap_size = 1024 * 1024 * 96;
//u32 __ctru_linear_heap_size = 1024 * 1024 * 8;
#elif __PS4__
extern "C" int sceSystemServiceLoadExec(const char *path, const char *args[]);
#endif

PSNESUiEmu *uiEmu;
PSNESConfig *cfg;
PSNESUIStateMenu *uiState;
RomList *romList;
UiMenu *uiMenu;

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
    auto *io = new PSNESIo();
    // load configuration
    int version = (__PSNES_VERSION_MAJOR__ * 100) + __PSNES_VERSION_MINOR__;
    cfg = new PSNESConfig(io, version);

    // create paths
    io->create(io->getDataPath());
    io->create(io->getDataPath() + "roms");
    io->create(io->getDataPath() + "configs");
    io->create(io->getDataPath() + "saves");

    Vector2f screenSize = cfg->getScreenSize();
    ui = new UiMain(screenSize, io, cfg);

#ifdef __SOFT_SCALERS__
    // TODO: fix
    ui->setShaderList(new ShaderList());
    ui->getShaderList()->add("TV2X", nullptr);
    ui->getShaderList()->add("SMOOTH", nullptr);
    ui->getShaderList()->add("SUPEREAGLE", nullptr);
    ui->getShaderList()->add("2XSAI", nullptr);
    ui->getShaderList()->add("SUPER2XSAI", nullptr);
    ui->getShaderList()->add("EPX", nullptr);
    ui->getShaderList()->add("HQ2X", nullptr);
#endif

    // skin
    skin = new Skin(ui);
    ui->setSkin(skin);

    // ui
    std::string snes9x_version = "snes9x: ";
    snes9x_version += VERSION;
    romList = new RomList(ui, snes9x_version, {".zip", ".sfc", ".smc", ".swc", ".fig"});
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new UiMenu(ui);
    uiEmu = new PSNESUiEmu(ui);
    uiState = new PSNESUIStateMenu(ui);
    ui->init(uiRomList, uiMenu, uiEmu, uiState);

    while (!ui->done) {
        ui->flip();
    }

    // cleanup
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

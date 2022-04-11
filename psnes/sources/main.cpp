#include "c2dui.h"
#include "uiEmu.h"
#include "uiMenu.h"
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

PSNESUIMenu *uiMenu;
PSNESUiEmu *uiEmu;
PSNESConfig *cfg;
PSNESUIStateMenu *uiState;
RomList *romList;

UiMain *ui;
Skin *skin;
UIRomList *uiRomList;

int main(int argc, char **argv) {

    // need custom io for some devices
    auto *io = new PSNESIo();
    // load configuration
    int version = (__PSNES_VERSION_MAJOR__ * 100) + __PSNES_VERSION_MINOR__;
    cfg = new PSNESConfig(io, version);

    // create paths
    io->create(io->getDataPath());
    io->create(io->getDataPath() + "roms");
    io->create(io->getDataPath() + "configs");

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

    // skin buttons used for config menu
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
    buttons.emplace_back(KEY_JOY_A_DEFAULT, "A");
    buttons.emplace_back(KEY_JOY_B_DEFAULT, "B");
    buttons.emplace_back(KEY_JOY_X_DEFAULT, "X");
    buttons.emplace_back(KEY_JOY_Y_DEFAULT, "Y");
    buttons.emplace_back(KEY_JOY_LT_DEFAULT, "ZL");
    buttons.emplace_back(KEY_JOY_RT_DEFAULT, "ZR");
    buttons.emplace_back(KEY_JOY_LB_DEFAULT, "L");
    buttons.emplace_back(KEY_JOY_RB_DEFAULT, "R");
    buttons.emplace_back(KEY_JOY_LS_DEFAULT, "LS");
    buttons.emplace_back(KEY_JOY_RS_DEFAULT, "RS");
    buttons.emplace_back(KEY_JOY_SELECT_DEFAULT, "-");
    buttons.emplace_back(KEY_JOY_START_DEFAULT, "+");
    buttons.emplace_back(KEY_JOY_MENU1_DEFAULT, "R");
    buttons.emplace_back(KEY_JOY_MENU2_DEFAULT, "L");
#elif __PS4__
    // see c2d.h for key id
    buttons.emplace_back(KEY_JOY_UP_DEFAULT, "UP");
    buttons.emplace_back(KEY_JOY_DOWN_DEFAULT, "DOWN");
    buttons.emplace_back(KEY_JOY_LEFT_DEFAULT, "LEFT");
    buttons.emplace_back(KEY_JOY_RIGHT_DEFAULT, "RIGHT");
    buttons.emplace_back(KEY_JOY_A_DEFAULT, "CROSS");
    buttons.emplace_back(KEY_JOY_B_DEFAULT, "CIRCLE");
    buttons.emplace_back(KEY_JOY_X_DEFAULT, "SQUARE");
    buttons.emplace_back(KEY_JOY_Y_DEFAULT, "TRIANGLE");
    buttons.emplace_back(KEY_JOY_LT_DEFAULT, "L2");
    buttons.emplace_back(KEY_JOY_RT_DEFAULT, "R2");
    buttons.emplace_back(KEY_JOY_LB_DEFAULT, "L2");
    buttons.emplace_back(KEY_JOY_RB_DEFAULT, "R2");
    buttons.emplace_back(KEY_JOY_LS_DEFAULT, "LS");
    buttons.emplace_back(KEY_JOY_RS_DEFAULT, "RS");
    buttons.emplace_back(KEY_JOY_SELECT_DEFAULT, "L1");
    buttons.emplace_back(KEY_JOY_START_DEFAULT, "R1");
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
    std::string snes9x_version = "snes9x: ";
    snes9x_version += VERSION;
    romList = new RomList(ui, snes9x_version, {".zip", ".sfc", ".smc", ".swc", ".fig"});
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new PSNESUIMenu(ui);
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

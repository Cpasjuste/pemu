#include "c2dui.h"
#include "uiEmu.h"
#include "uiMenu.h"
#include "uiStateMenu.h"
#include "config.h"
#include "psnesIo.h"
#include "snes9x.h"

#ifdef __PSP2__
#include <psp2/power.h>
#include <psp2/io/dirent.h>
int _newlib_heap_size_user = 192 * 1024 * 1024;
#elif __3DS__
//unsigned int __stacksize__ = 1024 * 1024;
//u32 __ctru_heap_size = 1024 * 1024 * 96;
//u32 __ctru_linear_heap_size = 1024 * 1024 * 8;
#endif

PSNESUIMenu *uiMenu;
PSNESUIEmu *uiEmu;
PSNESConfig *cfg;
PSNESUIStateMenu *uiState;
RomList *romList;

UIMain *ui;
Skin *skin;
UIRomList *uiRomList;

int main(int argc, char **argv) {

#ifdef __3DS__
    //consoleDebugInit(debugDevice_SVC);
    //stdout = stderr;
#endif

    // need custom io for some devices
    auto *io = new PSNESIo();
    // load configuration
    int version = (__PSNES_VERSION_MAJOR__ * 100) + __PSNES_VERSION_MINOR__;
    cfg = new PSNESConfig(io, version);

    // create paths
    io->create(io->getDataPath() + "configs");

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
    std::string snes9x_version = "snes9x: ";
    snes9x_version += VERSION;
    romList = new RomList(ui, snes9x_version);
    romList->build();
    uiRomList = new UIRomList(ui, romList, ui->getSize());
    uiMenu = new PSNESUIMenu(ui);
    uiEmu = new PSNESUIEmu(ui);
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
#endif

    return 0;
}

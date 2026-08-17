//
// Created by cpasjuste on 19/09/23.
//

#include "main.h"

using namespace c2d;
using namespace pemu;

PEMUUiMain *pemu_ui;

int main(int argc, char **argv) {
    // command line game info
    Game game;

    // custom io
    const auto io = new PEMUIo();

    // create main ui/renderer
    // NOTE: size must stay {0, 0} on non-switch platforms (PS4, PS5, Vita, Linux, Windows...).
    // libcross2d's SDL2Renderer only sets SDL_WINDOW_FULLSCREEN_DESKTOP when the requested
    // size is <= 0 (see SDL2Renderer::SDL2Renderer in libcross2d/source/platforms/sdl2/sdl2_renderer.cpp).
    // Passing a fixed size like {1280, 720} here creates a small, non-fullscreen SDL window,
    // which on PS4 (no windowing system / no desktop compositor) never actually gets
    // presented to the TV output -> black screen, even though audio keeps working fine
    // since it's an independent subsystem. Switch ignores this parameter entirely and
    // forces its own {1280, 720} internally (see UiMain's __SWITCH__ constructor), so it's
    // safe to leave this at {0, 0} for every platform.
    pemu_ui = new PEMUUiMain(Vector2f{0, 0});
    pemu_ui->setIo(io);

    // load configuration
    constexpr int version = (__PEMU_VERSION_MAJOR__ * 100) + __PEMU_VERSION_MINOR__;
    const auto cfg = new PEMUConfig(pemu_ui, version);
    pemu_ui->setConfig(cfg);

    // load skin configuration
    const auto skin = new PEMUSkin(pemu_ui);
    pemu_ui->setSkin(skin);

    // parse command line
    if (argc > 1) {
        if (io->exist(argv[1])) {
            game.path = Utility::baseName(argv[1]);
            game.name = Utility::removeExt(game.path);
            game.romsPath = Utility::remove(argv[1], game.path);
        } else {
            printf("main: file provided as console argument does not exist (%s)\n", argv[1]);
            delete (skin);
            delete (cfg);
            delete (pemu_ui);
            return 1;
        }
    }

    // ui
    const auto romList = new PEMURomList(pemu_ui, cfg->getCoreVersion(), cfg->getCoreSupportedExt());
    if (game.path.empty()) {
        romList->build();
        romList->initFav();
    } else {
        delete (romList->rect);
    }
    const auto uiRomList = new PEMUUiRomList(pemu_ui, romList, pemu_ui->getSize());
    const auto uiMenu = new PEMUUiMenu(pemu_ui);
    const auto uiEmu = new PEMUUiEmu(pemu_ui);
    const auto uiState = new PEMUUiMenuState(pemu_ui);
    pemu_ui->init(uiRomList, uiMenu, uiEmu, uiState);

    // load specified game from command line if requested
    if (!game.path.empty()) {
        uiRomList->setVisibility(Visibility::Hidden);
        uiRomList->setGames({game});
        cfg->loadGame(game);
        uiEmu->setExitOnStop(true);
        uiEmu->load(game);
    }

    while (!pemu_ui->done) {
        pemu_ui->flip();
    }

    delete (skin);
    delete (cfg);
    delete (pemu_ui);

#ifdef  __PS4__
    sceSystemServiceLoadExec((char *) "exit", nullptr);
    while (true) {}
#endif

    return 0;
}

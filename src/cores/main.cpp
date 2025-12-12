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
    auto io = new PEMUIo();

    // load configuration
    int pemu_version = (__PEMU_VERSION_MAJOR__ * 100) + __PEMU_VERSION_MINOR__;
    auto cfg = new PEMUConfig(io, pemu_version);

    // create main ui/renderer
    auto fs = cfg->get(PEMUConfig::OptId::UI_FULLSCREEN);
    pemu_ui = new PEMUUiMain(io, (fs && !fs->getInteger()) ? Vector2f{1280, 720} : Vector2f{0, 0});
    pemu_ui->setConfig(cfg);

    // load skin configuration
    auto skin = new PEMUSkin(pemu_ui);
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
    auto romList = new PEMURomList(pemu_ui, cfg->getCoreVersion(), cfg->getCoreSupportedExt());
    if (game.path.empty()) {
        romList->build();
        romList->initFav();
    } else {
        delete (romList->rect);
    }
    auto uiRomList = new PEMUUiRomList(pemu_ui, romList, pemu_ui->getSize());
    auto uiMenu = new PEMUUiMenu(pemu_ui);
    auto uiEmu = new PEMUUiEmu(pemu_ui);
    auto uiState = new PEMUUiMenuState(pemu_ui);
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

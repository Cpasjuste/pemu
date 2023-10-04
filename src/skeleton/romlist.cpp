//
// Created by cpasjuste on 19/10/16.
//

#include <vector>
#include <string>
#include <algorithm>
#include "pemu.h"
#include "romlist.h"


RomList::RomList(UiMain *_ui, const std::string &emuVersion, const std::vector<std::string> &_filters) {
    printf("RomList()\n");
    filters = _filters;
    ui = _ui;

    if (ui) {
        // UI
        rect = new C2DRectangle(
                Vector2f(ui->getSize().x - 8, ui->getSize().y - 8));
        ui->getSkin()->loadRectangleShape(rect, {"MAIN"});

        auto *title = new RectangleShape({16, 16});
        if (ui->getSkin()->loadRectangleShape(title, {"MAIN", "TITLE"})) {
            title->setOrigin(Origin::Center);
            title->setPosition(Vector2f(rect->getSize().x / 2, rect->getSize().y / 2));
            float scaling = std::min(
                    (rect->getSize().x * 0.8f) / title->getSize().x,
                    (rect->getSize().y * 0.8f) / title->getSize().y);
            title->setScale(scaling, scaling);
            rect->add(title);
        } else {
            delete (title);
        }

        text = new Text();
        ui->getSkin()->loadText(text, {"ROM_LIST", "TEXT"});
        text->setOrigin(Origin::BottomLeft);
        text->setPosition(8, rect->getSize().y - 8);
        rect->add(text);

        auto *version = new Text();
        ui->getSkin()->loadText(version, {"ROM_LIST", "TEXT"});
        version->setOrigin(Origin::BottomRight);
        version->setPosition(rect->getSize().x - 8, rect->getSize().y - 8);
        version->setString(emuVersion);
        rect->add(version);

        ui->add(rect);
    }

    gameList = new GameList();
    gameListFav = new GameList();
}

RomList::RomList(PEMUConfig *cfg, const std::string &emuVersion, const std::vector<std::string> &_filters) {
    printf("RomList(cfg)\n");
    mConfig = cfg;
    filters = _filters;
    gameList = new GameList();
    gameListFav = new GameList();
}

void RomList::build(const ss_api::GameList::GameAddedCb &cb) {
    float time_start = ui ? ui->getElapsedTime().asSeconds() : 0;
    auto cfg = mConfig ? mConfig : ui->getConfig();
    p_cb = cb;
    m_count = 0;

    if (ui) ui->flip();

    auto romPaths = cfg->getRomPaths();
    for (const auto &p: romPaths) {
        std::string gameListPath = p.path + "gamelist.xml";
        gameList->append(gameListPath, p.path, false, filters, p.system, false, [this](Game *game) {
            if (p_cb) p_cb(game);
            m_count++;
            if (!(m_count % 200)) {
                setLoadingText("Games: %li / %li", m_count, gameList->games.size());
            }
        });
        setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());
        printf("RomList::build: %s, games found: %zu / %zu (system: %s (0x%08x))\n",
               gameListPath.c_str(), gameList->getAvailableCount(),
               gameList->games.size(), p.system.name.c_str(), p.system.id);
    }

    // sort lists
    std::sort(gameList->systemList.systems.begin(), gameList->systemList.systems.end(), Api::sortSystemByName);
    std::sort(gameList->editors.begin(), gameList->editors.end(), Api::sortEditorByName);
    std::sort(gameList->developers.begin(), gameList->developers.end(), Api::sortDeveloperByName);
    std::sort(gameList->genres.begin(), gameList->genres.end(), Api::sortGenreByName);
    std::sort(gameList->players.begin(), gameList->players.end(), Api::sortInteger);
    std::sort(gameList->ratings.begin(), gameList->ratings.end(), Api::sortInteger);
    std::sort(gameList->rotations.begin(), gameList->rotations.end(), Api::sortInteger);
    std::sort(gameList->resolutions.begin(), gameList->resolutions.end(), Api::sortByName);
    std::sort(gameList->dates.begin(), gameList->dates.end(), Api::sortByName);

    // add filtering options
    auto grp = cfg->getGroup(PEMUConfig::GrpId::UI_FILTERING);
    grp->getOption(PEMUConfig::OptId::UI_FILTER_SYSTEM)->setArray(gameList->systemList.getNames(), 0);
    grp->getOption(PEMUConfig::OptId::UI_FILTER_GENRE)->setArray(gameList->getGenreNames(), 0);
    grp->getOption(PEMUConfig::OptId::UI_FILTER_DATE)->setArray(gameList->getDates(), 0);
    grp->getOption(PEMUConfig::OptId::UI_FILTER_EDITOR)->setArray(gameList->getEditorNames(), 0);
    grp->getOption(PEMUConfig::OptId::UI_FILTER_DEVELOPER)->setArray(gameList->getDeveloperNames(), 0);
    grp->getOption(PEMUConfig::OptId::UI_FILTER_PLAYERS)->setArray(gameList->getPlayersNames(), 0);
    grp->getOption(PEMUConfig::OptId::UI_FILTER_RATING)->setArray(gameList->getRatingNames(), 0);

    // custom core hide/show flags
    auto ids = cfg->getCoreHiddenOptionToEnable();
    for (const auto &id: ids) {
        cfg->get(id)->setFlags(0);
    }

    // we need to reload config to update new options we just added
    cfg->load();

    if (ui) printf("RomList::build(): list built in %f\n", ui->getElapsedTime().asSeconds() - time_start);

    // remove ui widgets
    if (rect) delete (rect);
}

void RomList::initFav() {
    // load favorites
    delete (gameListFav);
    gameListFav = new GameList();
    gameListFav->append(ui->getIo()->getDataPath() + "favorites.xml");
    for (auto &g: gameListFav->games) {
        auto game = gameList->findGameByPathAndSystem(g.path, g.system.id);
        if (!game.path.empty()) {
            g.available = true;
            g.romsPath = game.romsPath;
        }
    }

    printf("RomList::initFav: %zu favorites\n", gameListFav->games.size());
}

void RomList::addFav(const Game &game) {
    if (!gameListFav->exist(game.id)) {
        gameListFav->games.emplace_back(game);
        gameListFav->save(ui->getIo()->getDataPath() + "favorites.xml", "mixrbv2", "", "video");
    }
}

void RomList::removeFav(const Game &game) {
    if (gameListFav->remove(game.id)) {
        gameListFav->save(ui->getIo()->getDataPath() + "favorites.xml", "mixrbv2", "", "video");
    }
}

void RomList::setLoadingText(const char *format, ...) {
    if (!ui) return;

    char buffer[512];
    va_list arg;
    va_start(arg, format);
    vsnprintf(buffer, 512, format, arg);
    va_end(arg);

    text->setString(buffer);
    ui->flip();
}

RomList::~RomList() {
    printf("~RomList()\n");
    delete (gameList);
    delete (gameListFav);
}

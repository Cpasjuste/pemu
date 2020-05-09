//
// Created by cpasjuste on 19/10/16.
//

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "c2dui.h"

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

RomList::RomList(UIMain *_ui, const std::string &emuVersion) {

    printf("RomList()\n");

    ui = _ui;
    paths = ui->getConfig()->getRomPaths();

    // UI
    rect = new C2DRectangle(
            Vector2f(ui->getSize().x - 8, ui->getSize().y - 8));
    ui->getSkin()->loadRectangleShape(rect, {"MAIN"});

    auto title = new RectangleShape({16, 16});
    ui->getSkin()->loadRectangleShape(title, {"MAIN", "TITLE"});
    title->setOrigin(Origin::Center);
    title->setPosition(Vector2f(rect->getSize().x / 2, rect->getSize().y / 2));
    float scaling = std::min(
            (rect->getSize().x - 256) / title->getSize().x,
            (rect->getSize().y - 256) / title->getSize().y);
    title->setScale(scaling, scaling);
    rect->add(title);

    //auto font_size = (unsigned int) ((float) C2D_DEFAULT_CHAR_SIZE * ((float) C2D_SCREEN_HEIGHT / 720.0f));
    text = new Text("Games: 0/0", C2D_DEFAULT_CHAR_SIZE, ui->getSkin()->font);
    ui->getSkin()->loadText(text, {"ROM_LIST", "TEXT"});
    text->setOrigin(Origin::BottomLeft);
    text->setPosition(8, rect->getSize().y - ((float) text->getCharacterSize() / 2));
    rect->add(text);

    auto *version = new Text(emuVersion, C2D_DEFAULT_CHAR_SIZE, ui->getSkin()->font);
    ui->getSkin()->loadText(version, {"ROM_LIST", "TEXT"});
    version->setOrigin(Origin::BottomRight);
    version->setPosition(rect->getSize().x - 16, rect->getSize().y - ((float) text->getCharacterSize() / 2));
    rect->add(version);

    ui->add(rect);
    ui->flip();
    // UI

    printf("RomList: building list...\n");
    time_start = ui->getElapsedTime().asSeconds();

    printf("RomList()\n");
}

void RomList::setLoadingText(const char *format, ...) {

    char buffer[512];
    va_list arg;
    va_start(arg, format);
    vsnprintf(buffer, 512, format, arg);
    va_end(arg);

    text->setString(buffer);
    ui->flip();
}

void RomList::build() {

    printf("RomList::build(): ROM_PATH_0: %s\n", ui->getConfig()->getRomPaths().at(FBN_PATH_ARCADE).c_str());
    std::string dataPath = ui->getIo()->getDataPath();

#ifdef __PFBA__
    gameList.append(dataPath + "gamelist.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_ARCADE), false);
#else
    gameList.append(dataPath + "gamelist.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_ARCADE), false, true);
#endif
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());
    printf("RomList::build: %i roms\n", gameList.games.size());

    gameList.systems.insert(gameList.systems.begin(), "ALL");
    gameList.editors.insert(gameList.editors.begin(), "ALL");
    gameList.developers.insert(gameList.developers.begin(), "ALL");
    gameList.players.insert(gameList.players.begin(), "ALL");
    gameList.ratings.insert(gameList.ratings.begin(), "ALL");
    gameList.topStaffs.insert(gameList.topStaffs.begin(), "ALL");
    gameList.rotations.insert(gameList.rotations.begin(), "ALL");
    gameList.resolutions.insert(gameList.resolutions.begin(), "ALL");
    gameList.dates.insert(gameList.dates.begin(), "ALL");
    gameList.genres.insert(gameList.genres.begin(), "ALL");

#ifdef __PFBA__
    ui->getConfig()->add(
            Option::Id::GUI_SHOW_ROM_NAMES, "FILTER_SYSTEM",
            gameList.systems, 0, Option::Id::GUI_FILTER_SYSTEM, Option::Flags::STRING);
#else
    ui->getConfig()->add(
            Option::Id::GUI_SHOW_ROM_NAMES, "FILTER_SYSTEM",
            gameList.systems, 0, Option::Id::GUI_FILTER_SYSTEM, Option::Flags::STRING | Option::Flags::HIDDEN);
#endif

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_SYSTEM, "FILTER_EDITOR",
            gameList.editors, 0, Option::Id::GUI_FILTER_EDITOR, Option::Flags::STRING);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_EDITOR, "FILTER_DEVELOPER",
            gameList.developers, 0, Option::Id::GUI_FILTER_DEVELOPER, Option::Flags::STRING);

#ifdef __PFBA__
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_DEVELOPER, "FILTER_PLAYERS",
            gameList.players, 0, Option::Id::GUI_FILTER_PLAYERS, Option::Flags::STRING);
#else
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_DEVELOPER, "FILTER_PLAYERS",
            gameList.players, 0, Option::Id::GUI_FILTER_PLAYERS, Option::Flags::STRING | Option::Flags::HIDDEN);
#endif

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_PLAYERS, "FILTER_RATING",
            gameList.ratings, 0, Option::Id::GUI_FILTER_RATING, Option::Flags::STRING);

#ifdef __PFBA__
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_RATING, "FILTER_ROTATION",
            gameList.rotations, 0, Option::Id::GUI_FILTER_ROTATION, Option::Flags::STRING);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_ROTATION, "FILTER_RESOLUTION",
            gameList.resolutions, 0, Option::Id::GUI_FILTER_RESOLUTION, Option::Flags::STRING);
#else
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_RATING, "FILTER_ROTATION",
            gameList.rotations, 0, Option::Id::GUI_FILTER_ROTATION, Option::Flags::STRING | Option::Flags::HIDDEN);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_ROTATION, "FILTER_RESOLUTION",
            gameList.resolutions, 0, Option::Id::GUI_FILTER_RESOLUTION, Option::Flags::STRING | Option::Flags::HIDDEN);
#endif

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_RESOLUTION, "FILTER_DATE",
            gameList.dates, 0, Option::Id::GUI_FILTER_DATE, Option::Flags::STRING);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_DATE, "FILTER_GENRE",
            gameList.genres, 0, Option::Id::GUI_FILTER_GENRE, Option::Flags::STRING);

    // we need to reload config to update new options we just added
    ui->getConfig()->reset();
    ui->getConfig()->load();

    gameListFav = GameList(dataPath + "favorites.xml");
    for (size_t i = 0; i < gameListFav.games.size(); i++) {
        Game game = gameList.findByPathAndSystem(gameListFav.games[i].path, gameListFav.games[i].system.id);
        if (!game.path.empty()) {
            gameListFav.games[i].available = true;
            gameListFav.games[i].romsPath = game.romsPath;
        }
    }
    printf("RomList::build: %i favorites\n", gameListFav.games.size());

    float time_spent = ui->getElapsedTime().asSeconds() - time_start;
    printf("RomList::build(): list built in %f\n", time_spent);

    // UI
    // remove ui widgets
    delete (rect);
}

void RomList::addFav(const Game &game) {

    if (!gameListFav.exist(game.romId)) {
        gameListFav.games.emplace_back(game);
        gameListFav.save(ui->getIo()->getDataPath() + "favorites.xml",
                         Game::Language::EN, GameList::Format::ScreenScrapper,
                         {"mixrbv2", "video"});
    }
}

void RomList::removeFav(const Game &game) {

    if (gameListFav.remove(game.romId)) {
        gameListFav.save(ui->getIo()->getDataPath() + "favorites.xml",
                         Game::Language::EN, GameList::Format::ScreenScrapper,
                         {"mixrbv2", "video"});
    }
}

RomList::~RomList() {
    printf("~RomList()\n");
}

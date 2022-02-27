//
// Created by cpasjuste on 19/10/16.
//

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include "c2dui.h"

RomList::RomList(UiMain *_ui, const std::string &emuVersion) {

    printf("RomList()\n");

    ui = _ui;
    paths = ui->getConfig()->getRomPaths();

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

    //auto font_size = (unsigned int) ((float) C2D_DEFAULT_CHAR_SIZE * ((float) C2D_SCREEN_HEIGHT / 720.0f));
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

    std::string romPath = ui->getConfig()->getRomPaths().at(FBN_PATH_ARCADE);
    printf("RomList::build(): ROM_PATH_0: %s\n", romPath.c_str());

    std::string gameListPath = ui->getIo()->getRomFsPath() + "gamelist.xml";
    if (!ui->getIo()->exist(gameListPath)) {
        gameListPath = ui->getIo()->getDataPath() + "gamelist.xml";
    }

    gameList.append(gameListPath,
                    ui->getConfig()->getRomPaths().at(FBN_PATH_ARCADE), false, true);

    setLoadingText("Games: %li / %li", gameList.getAvailableCount(), gameList.games.size());
    printf("RomList::build: games: %li / %li\n", gameList.getAvailableCount(), gameList.games.size());

    // sort lists
    std::sort(gameList.systems.begin(), gameList.systems.end(), Api::sortByName);
    std::sort(gameList.editors.begin(), gameList.editors.end(), Api::sortByName);
    std::sort(gameList.developers.begin(), gameList.developers.end(), Api::sortByName);
    std::sort(gameList.players.begin(), gameList.players.end(), Api::sortByName);
    std::sort(gameList.ratings.begin(), gameList.ratings.end(), Api::sortByName);
    std::sort(gameList.topStaffs.begin(), gameList.topStaffs.end(), Api::sortByName);
    std::sort(gameList.rotations.begin(), gameList.rotations.end(), Api::sortByName);
    std::sort(gameList.resolutions.begin(), gameList.resolutions.end(), Api::sortByName);
    std::sort(gameList.dates.begin(), gameList.dates.end(), Api::sortByName);
    std::sort(gameList.genres.begin(), gameList.genres.end(), Api::sortByName);

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

    ui->getConfig()->add(
            Option::Id::GUI_SHOW_ZIP_NAMES, "FILTER_SYSTEM",
            gameList.systems, 0, Option::Id::GUI_FILTER_SYSTEM, Option::Flags::STRING | Option::Flags::HIDDEN);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_SYSTEM, "FILTER_EDITOR",
            gameList.editors, 0, Option::Id::GUI_FILTER_EDITOR, Option::Flags::STRING);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_EDITOR, "FILTER_DEVELOPER",
            gameList.developers, 0, Option::Id::GUI_FILTER_DEVELOPER, Option::Flags::STRING);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_DEVELOPER, "FILTER_PLAYERS",
            gameList.players, 0, Option::Id::GUI_FILTER_PLAYERS, Option::Flags::STRING);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_PLAYERS, "FILTER_RATING",
            gameList.ratings, 0, Option::Id::GUI_FILTER_RATING, Option::Flags::STRING);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_RATING, "FILTER_ROTATION",
            gameList.rotations, 0, Option::Id::GUI_FILTER_ROTATION, Option::Flags::STRING | Option::Flags::HIDDEN);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_ROTATION, "FILTER_RESOLUTION",
            gameList.resolutions, 0, Option::Id::GUI_FILTER_RESOLUTION, Option::Flags::STRING | Option::Flags::HIDDEN);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_RESOLUTION, "FILTER_DATE",
            gameList.dates, 0, Option::Id::GUI_FILTER_DATE, Option::Flags::STRING);
    ui->getConfig()->add(
            Option::Id::GUI_FILTER_DATE, "FILTER_GENRE",
            gameList.genres, 0, Option::Id::GUI_FILTER_GENRE, Option::Flags::STRING);

    // we need to reload config to update new options we just added
    ui->getConfig()->reset();
    ui->getConfig()->load();

    gameListFav = GameList(ui->getIo()->getDataPath() + "favorites.xml");
    for (size_t i = 0; i < gameListFav.games.size(); i++) {
        Game game = gameList.findByPathAndSystem(gameListFav.games[i].path, gameListFav.games[i].system.id);
        if (!game.path.empty()) {
            gameListFav.games[i].available = true;
            gameListFav.games[i].romsPath = game.romsPath;
        }
    }
    printf("RomList::build: %lu favorites\n", gameListFav.games.size());

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

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

    printf("RomList::build()\n");

    gameList = GameList("gamelist.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_ARCADE));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_coleco.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_COLECO));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_gamegear.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_GAMEGEAR));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_megadriv.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_MEGADRIV));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_msx.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_MSX));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_pce.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_PCE));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_sg1000.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_SG1000));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_sgx.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_SGX));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_sms.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_SMS));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_tg16.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_TG16));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    gameList.append("gamelist_zx3.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_ZX3));
    setLoadingText("Games: %i / %zu", gameList.getAvailableCount(), gameList.games.size());
    printf("RomList::build: %zu roms\n", gameList.games.size());

    ui->delay(1000);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_CLONES, "FILTER_SYSTEM",
            gameList.systems, 0, Option::Id::GUI_FILTER_SYSTEM, Option::Flags::STRING);

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
            gameList.rotations, 0, Option::Id::GUI_FILTER_ROTATION, Option::Flags::STRING);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_ROTATION, "FILTER_RESOLUTION",
            gameList.resolutions, 0, Option::Id::GUI_FILTER_RESOLUTION, Option::Flags::STRING);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_RESOLUTION, "FILTER_DATE",
            gameList.dates, 0, Option::Id::GUI_FILTER_DATE, Option::Flags::STRING);

    ui->getConfig()->add(
            Option::Id::GUI_FILTER_DATE, "FILTER_GENRE",
            gameList.genres, 0, Option::Id::GUI_FILTER_GENRE, Option::Flags::STRING);

    // we need to reload config to update new options we just added
    ui->getConfig()->reset();
    ui->getConfig()->load();

    gameListFav = GameList("favorites.xml", ui->getConfig()->getRomPaths().at(0));
    printf("RomList::build: %zu favorites\n", gameListFav.games.size());

    float time_spent = ui->getElapsedTime().asSeconds() - time_start;
    printf("RomList::build(): list built in %f\n", time_spent);

    // UI
    // remove ui widgets
    delete (rect);
}

void RomList::addFav(const Game &game) {

    if (!gameListFav.exist(game.romId)) {
        gameListFav.games.emplace_back(game);
        gameListFav.save("favorites.xml", Game::Language::EN, GameList::Format::ScreenScrapper);
    }
}

void RomList::removeFav(const Game &game) {

    if (gameListFav.remove(game.romId)) {
        gameListFav.save("favorites.xml", Game::Language::EN, GameList::Format::ScreenScrapper);
    }
}

RomList::~RomList() {
    printf("~RomList()\n");
}

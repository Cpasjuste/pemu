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

    strcpy(text_str, "Roms found: 0/0");
    //auto font_size = (unsigned int) ((float) C2D_DEFAULT_CHAR_SIZE * ((float) C2D_SCREEN_HEIGHT / 720.0f));
    text = new Text(text_str, C2D_DEFAULT_CHAR_SIZE, ui->getSkin()->font);
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

void RomList::build() {

    printf("RomList::build()\n");

    gameList = GameList("gamelist.xml", ui->getConfig()->getRomPaths().at(0));
    printf("RomList::build: %zu roms\n", gameList.games.size());

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

    /*
     * TODO: add/fix favorites back
    // build favorites
    std::string favPath = ui->getConfig()->getHomePath() + "favorites.bin";
    std::ifstream favFile(favPath);
    if (favFile.is_open()) {
        std::string line;
        while (std::getline(favFile, line)) {
            auto rom = std::find_if(list.begin(), list.end(), [line](Rom *rom) -> bool {
                return line == rom->path;
            });
            if (rom != list.end()) {
                (*rom)->hardware |= HARDWARE_PREFIX_FAV;
            }
        }
        favFile.close();
    }
    */

    float time_spent = ui->getElapsedTime().asSeconds() - time_start;
    printf("RomList::build(): list built in %f\n", time_spent);

    // UI
    // remove ui widgets
    delete (rect);
}

void RomList::addFav(Game *rom) {

    /*
    if (!rom || rom->hardware & HARDWARE_PREFIX_FAV) {
        printf("RomList::addFav: already in favorites\n");
        return;
    }

    rom->hardware |= HARDWARE_PREFIX_FAV;

    std::string favPath = ui->getConfig()->getHomePath() + "favorites.bin";
    std::ofstream favFile(favPath, std::ios::app);
    if (favFile.is_open()) {
        favFile << rom->path;
        favFile << "\n";
        favFile.close();
    }
    */
}

void RomList::removeFav(Game *rom) {

    /*
    if (!rom || !(rom->hardware & HARDWARE_PREFIX_FAV)) {
        printf("RomList::addFav: not in favorites\n");
        return;
    }

    rom->hardware &= ~HARDWARE_PREFIX_FAV;

    // TODO: only remove specific line ?
    std::string favPath = ui->getConfig()->getHomePath() + "favorites.bin";
    std::ofstream favFile(favPath, std::ios::trunc);
    if (favFile.is_open()) {
        for (auto &r : list) {
            if (r->flags & HARDWARE_PREFIX_FAV) {
                favFile << rom->path;
                favFile << "\n";
            }
        }
        favFile.close();
    }
    */
}

RomList::~RomList() {

    printf("~RomList()\n");

    /*
    for (auto &rom : list.games) {
        if (rom && !rom->parent && rom->icon) {
            delete (rom->icon);
            rom->icon = nullptr;
        }
        if (rom) {
            delete (rom);
        }
    }
    */
}

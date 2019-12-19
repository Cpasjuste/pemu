//
// Created by cpasjuste on 22/11/16.
//
#include <algorithm>

#include "c2dui.h"

/// pFBA
#define BDF_ORIENTATION_FLIPPED     (1 << 1)
#define BDF_ORIENTATION_VERTICAL    (1 << 2)

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

UIRomList::UIRomList(UIMain *u, RomList *rList, const c2d::Vector2f &size) : RectangleShape(size) {

    printf("UIRomList\n");

    ui = u;
    romList = rList;
    gameList = romList->gameList;
}

Game UIRomList::getSelection() {
    return Game();
}

RomList *UIRomList::getRomList() {
    return romList;
}

Texture *UIRomList::getPreviewTexture(const ss_api::Game &game) {

    // load image
    C2DTexture *texture = nullptr;
    std::string path;

    path = game.romsPath + game.getMedia("mixrbv2").url;
    printf("getPreviewTexture(%s)\n", path.c_str());
    texture = new C2DTexture(path);
#ifndef __SWITCH__
    // TODO: fix switch stat/fopen slow on non existing files
    if (!texture->available) {
        delete (texture);
        texture = nullptr;
        if (game.isClone()) {
            Game parentGame = gameList.findByPath(game.cloneOf);
            if (!parentGame.path.empty()) {
                path = game.romsPath + parentGame.getMedia("mixrbv2").url;
                printf("getPreviewTexture(%s)\n", path.c_str());
                texture = new C2DTexture(path);
                if (!texture->available) {
                    delete (texture);
                    return nullptr;
                }
            }
        }
    }
#endif

    return texture;
}

void UIRomList::filterRomList() {

    Option *opt = ui->getConfig()->get(Option::Id::GUI_SHOW_ALL);
    if (opt->getValueString() == "FAVORITES") {
        gameList = romList->gameListFav.filter(
                false,
                ui->getConfig()->get(Option::Id::GUI_FILTER_CLONES)->getValueBool(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_EDITOR)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_DEVELOPER)->getValueString(),

                ui->getConfig()->get(Option::Id::GUI_FILTER_PLAYERS)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_RATING)->getValueString(),
                "All",
                ui->getConfig()->get(Option::Id::GUI_FILTER_ROTATION)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_RESOLUTION)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_DATE)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_GENRE)->getValueString()
        );
    } else {
        gameList = romList->gameList.filter(
                ui->getConfig()->get(Option::Id::GUI_SHOW_ALL)->getValueString() == "AVAILABLE",
                ui->getConfig()->get(Option::Id::GUI_FILTER_CLONES)->getValueBool(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_EDITOR)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_DEVELOPER)->getValueString(),

                ui->getConfig()->get(Option::Id::GUI_FILTER_PLAYERS)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_RATING)->getValueString(),
                "All",
                ui->getConfig()->get(Option::Id::GUI_FILTER_ROTATION)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_RESOLUTION)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_DATE)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_GENRE)->getValueString()
        );
    }
}

void UIRomList::setVideoSnapDelay(int delay) {
    timer_load_video_delay = delay * 1000;
}

void UIRomList::updateRomList() {
}

UIRomList::~UIRomList() {
    printf("~UIRomList\n");
    delete (romList);
}

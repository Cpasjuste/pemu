//
// Created by cpasjuste on 22/11/16.
//
#include <algorithm>

#include "c2dui.h"
#include "c2dui_ui_romlist.h"

#ifdef __PFBA__

#include "../pfba/sources/fba/tchar.h"
#include "../cores/fba/src/burn/burn.h"

#endif

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
    std::string fullPath, mediaPath;

    // if database doesn't contains any media, just use default path
    mediaPath = game.getMedia("mixrbv2").url;
    if (mediaPath.empty()) {
        mediaPath = "media/mixrbv2/" + Utility::removeExt(game.path) + ".png";
    }

    fullPath = game.romsPath + mediaPath;
    printf("getPreviewTexture(%s)\n", fullPath.c_str());
    texture = new C2DTexture(fullPath);
    if (!texture->available) {
        delete (texture);
        texture = nullptr;
        if (game.isClone()) {
            Game parentGame = gameList.findByPath(game.cloneOf);
            if (!parentGame.path.empty()) {
                mediaPath = parentGame.getMedia("mixrbv2").url;
                if (mediaPath.empty()) {
                    mediaPath = "media/mixrbv2/" + Utility::removeExt(parentGame.path) + ".png";
                }
                fullPath = game.romsPath + mediaPath;
                printf("getPreviewTexture(%s)\n", fullPath.c_str());
                texture = new C2DTexture(fullPath);
                if (!texture->available) {
                    delete (texture);
                    texture = nullptr;
                }
            }
        } else {
            // for non arcade game, search for a "screenscraper" game with same name
            std::vector<Game> clones = gameList.findByName(game);
            for (const auto &g : clones) {
                mediaPath = g.getMedia("mixrbv2").url;
                fullPath = g.romsPath + mediaPath;
                texture = new C2DTexture(fullPath);
                if (!texture->available) {
                    delete (texture);
                    texture = nullptr;
                } else {
                    break;
                }
            }
        }
    }

    return texture;
}

std::string UIRomList::getPreviewVideo(const ss_api::Game &game) {

    std::string fullPath, mediaPath;

    // if database doesn't contains any media, just use default path
    mediaPath = game.getMedia("video").url;
    if (mediaPath.empty()) {
        mediaPath = "media/video/" + Utility::removeExt(game.path) + ".mp4";
    }

    fullPath = game.romsPath + mediaPath;
    printf("getPreviewVideo(%s)\n", fullPath.c_str());
    if (!ui->getIo()->exist(fullPath)) {
        fullPath = "";
        Game parentGame = gameList.findByPath(game.cloneOf);
        if (!parentGame.path.empty()) {
            mediaPath = parentGame.getMedia("video").url;
            if (mediaPath.empty()) {
                mediaPath = "media/video/" + Utility::removeExt(parentGame.path) + ".mp4";
            }
            fullPath = game.romsPath + mediaPath;
            printf("getPreviewVideo(%s)\n", fullPath.c_str());
            if (!ui->getIo()->exist(fullPath)) {
                fullPath = "";
            }
        } else {
            // for non arcade game, search for a "screenscraper" game with same name
            std::vector<Game> clones = gameList.findByName(game);
            for (const auto &g : clones) {
                mediaPath = g.getMedia("video").url;
                fullPath = game.romsPath + mediaPath;
                if (!ui->getIo()->exist(fullPath)) {
                    fullPath = "";
                } else {
                    break;
                }
            }
        }
    }

    return fullPath;
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
                "ALL",
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
                "ALL",
                ui->getConfig()->get(Option::Id::GUI_FILTER_ROTATION)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_RESOLUTION)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_DATE)->getValueString(),
                ui->getConfig()->get(Option::Id::GUI_FILTER_GENRE)->getValueString()
        );
    }

    bool byPath = ui->getConfig()->get(Option::Id::GUI_SHOW_ROM_NAMES)->getValueBool();
#ifdef __PFBA__
    if (byPath) {
        for (unsigned int i = 0; i < nBurnDrvCount; i++) {
            nBurnDrvActive = i;
            std::string fbnZip = std::string(BurnDrvGetTextA(DRV_NAME)) + ".zip";
            std::string fbnName = BurnDrvGetTextA(DRV_FULLNAME);
            char *z_name = nullptr;
            BurnDrvGetZipName(&z_name, 0);
            if (z_name != nullptr) {
                fbnZip = std::string(z_name) + ".zip";
            }
            for (size_t j = 0; j < gameList.games.size(); j++) {
                if (gameList.games.at(j).path == fbnZip) {
                    // FBNEO add an header to nes and fds rom names..
                    unsigned int hw =
                            ((BurnDrvGetHardwareCode() | HARDWARE_PREFIX_CARTRIDGE) ^ HARDWARE_PREFIX_CARTRIDGE) &
                            0xff000000;
                    if (hw == HARDWARE_NES) {
                        if (Utility::startWith(fbnName, "NES ")) {
                            fbnName = Utility::remove(fbnName, "NES ");
                        }
                    } else if (hw == HARDWARE_FDS) {
                        if (Utility::startWith(fbnName, "FDS ")) {
                            fbnName = Utility::remove(fbnName, "FDS ");
                        }
                    }
                    gameList.games.at(j).names[0].text = fbnName;
                    break;
                }
            }
        }
    }
#endif

    gameList.sortAlpha(byPath);
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


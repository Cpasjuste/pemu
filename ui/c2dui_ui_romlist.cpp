//
// Created by cpasjuste on 22/11/16.
//
#include <algorithm>
#include "c2dui.h"
#include "ss_api.h"

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

UIRomList::UIRomList(UIMain *u, RomList *rList, const c2d::Vector2f &size) : RectangleShape(size) {

    printf("UIRomList\n");

    ui = u;
    romList = rList;
    gameList = romList->gameList;
    Skin *skin = ui->getSkin();

    // set gui main "window"
    skin->loadRectangleShape(this, {"MAIN"});

    // add title image if available
    auto *title = new RectangleShape({16, 16});
    skin->loadRectangleShape(title, {"MAIN", "TITLE"});
    UIRomList::add(title);

    // add help image if available
    auto *help = new RectangleShape({16, 16});
    if (skin->loadRectangleShape(help, {"MAIN", "HELP"})) {
        UIRomList::add(help);
    } else {
        delete (help);
    }

    // add rom info ui
    romInfo = new UIRomInfo(ui, this, skin->font, ui->getFontSize());
    UIRomList::add(romInfo);

    int delay = ui->getConfig()->get(Option::Id::GUI_VIDEO_SNAP_DELAY)->getValueInt();
    UIRomList::setVideoSnapDelay(delay);

    // filter roms
    UIRomList::updateRomList();
}

void UIRomList::updateRomList() {

    filterRomList();
    sortRomList();

    Skin::TextGroup textGroup = ui->getSkin()->getText({"MAIN", "ROM_LIST", "TEXT"});
    config::Group *grp = ui->getSkin()->getConfig()->getGroup("ROM_LIST")->getGroup("TEXT");
    Color colorMissing = grp->getOption("color_missing")->getColor();
    bool highlightUseFileColors = grp->getOption("highlight_use_text_color")->getInteger() == 1;

    if (listBox == nullptr) {
        // add rom list ui
        Skin::RectangleShapeGroup romListGroup = ui->getSkin()->getRectangleShape({"MAIN", "ROM_LIST"});
        bool use_icons = false;
#if !(defined(__PSP2__) || defined(__3DS__)) // two slow
        use_icons = ui->getConfig()->get(Option::Id::GUI_SHOW_ICONS)->getValueBool();
#endif
        listBox = new UIListBox(ui, ui->getSkin()->font, (int) textGroup.size,
                                romListGroup.rect, gameList.games, use_icons);
        listBox->colorMissing = colorMissing;
        listBox->colorAvailable = textGroup.color;
        listBox->setFillColor(romListGroup.color);
        listBox->setOutlineColor(romListGroup.outlineColor);
        listBox->setOutlineThickness((float) romListGroup.outlineSize);
        listBox->setSelection(0);
        // rom item
        listBox->setTextOutlineColor(textGroup.outlineColor);
        listBox->setTextOutlineThickness((float) textGroup.outlineSize);
        // hihglight
        Skin::RectangleShapeGroup rectShape = ui->getSkin()->getRectangleShape({"SKIN_CONFIG", "HIGHLIGHT"});
        listBox->getHighlight()->setFillColor(rectShape.color);
        listBox->getHighlight()->setOutlineColor(rectShape.outlineColor);
        listBox->getHighlight()->setOutlineThickness((float) rectShape.outlineSize);
        listBox->setHighlightUseFileColor(highlightUseFileColors);
        add(listBox);
    } else {
        listBox->setGames(gameList.games);
    }

    if (romInfo != nullptr) {
        romInfo->load(Game());
        timer_load_info_done = 0;
        timer_load_info.restart();
        timer_load_video_done = 0;
        timer_load_video.restart();
    }
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
}

void UIRomList::sortRomList() {
    bool byPath = ui->getConfig()->get(Option::Id::GUI_SHOW_ROM_NAMES)->getValueBool();
    gameList.sortAlpha(byPath);
}

void UIRomList::setVideoSnapDelay(int delay) {
    timer_load_video_delay = delay * 1000;
}

Game UIRomList::getSelection() {
    return listBox->getSelection();
}

RomList *UIRomList::getRomList() {
    return romList;
}

void UIRomList::setVisibility(c2d::Visibility visibility, bool tweenPlay) {

    if (visibility == c2d::Visibility::Hidden) {
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else {
        timer_load_info.restart();
        timer_load_video.restart();
    }

    RectangleShape::setVisibility(visibility, tweenPlay);
}

bool UIRomList::onInput(c2d::Input::Player *players) {

    if (ui->getUiMenu()->isVisible()
        || ui->getUiStateMenu()->isVisible()
        || ui->getUiProgressBox()->isVisible()) {
        return false;
    }

    unsigned int keys = players[0].keys;
    if (keys & Input::Key::Up) {
        listBox->up();
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (keys & Input::Key::Down) {
        listBox->down();
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (keys & Input::Key::Right) {
        listBox->setSelection(listBox->getIndex() + listBox->getMaxLines());
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (keys & Input::Key::Left) {
        listBox->setSelection(listBox->getIndex() - listBox->getMaxLines());
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (keys & Input::Key::Fire1) {
        Game game = getSelection();
        if (game.available) {
#ifdef __MPV__
            romInfo->mpvTexture->setVisibility(c2d::Visibility::Hidden);
            romInfo->mpv->stop();
#endif
            ui->getConfig()->load(game);
            ui->getUiEmu()->load(game);
            return true;
        }
    } else if (keys & Input::Key::Fire3) {
        // add to favorites
        Game game = getSelection();
        if (game.id > 0 && !romList->gameListFav.exist(game.romId)) {
            int res = ui->getUiMessageBox()->show("FAVORITES",
                                                  "Add to favorites ?",
                                                  "OK", "CANCEL");
            if (res == MessageBox::LEFT) {
                romList->addFav(game);
            }
        } else if (game.id > 0 && romList->gameListFav.exist(game.romId)) {
            int res = ui->getUiMessageBox()->show("FAVORITES",
                                                  "Remove from favorites ?",
                                                  "OK", "CANCEL");
            if (res == MessageBox::LEFT) {
                romList->removeFav(game);
                Option *opt = ui->getConfig()->get(Option::Id::GUI_SHOW_ALL);
                if (opt->getValueString() == "FAVORITES") {
                    // update list if we are in favorites
                    updateRomList();
                }
            }
        }
    } else if (keys & Input::Key::Start) {
        ui->getUiMenu()->load();
    } else if (keys & Input::Key::Select) {
        if (getSelection().id > 0) {
            ui->getConfig()->load(getSelection());
            ui->getUiMenu()->load(true);

        }
    } else if (keys & EV_QUIT) {
        ui->done = true;
    }

    return true;
}

void UIRomList::onUpdate() {

    if (!isVisible() || ui->getUiProgressBox()->isVisible()) {
        return;
    }

    RectangleShape::onUpdate();

    unsigned int keys = ui->getInput()->getKeys();

    if (keys > 0 && keys != Input::Delay) {
        timer_load_info.restart();
        timer_load_video.restart();
    } else if (keys == 0) {
        if ((timer_load_info_done == 0) && timer_load_info.getElapsedTime().asMilliseconds() > timer_load_info_delay) {
            romInfo->load(listBox->getSelection());
            timer_load_info_done = 1;
        }
        if (timer_load_video_delay > 0 && timer_load_video_done == 0 &&
            timer_load_video.getElapsedTime().asMilliseconds() > timer_load_video_delay) {
            romInfo->loadVideo(listBox->getSelection());
            timer_load_video_done = 1;
        }
    }
}

UIRomList::~UIRomList() {
    printf("~UIRomListClassic\n");
    delete (romList);
}

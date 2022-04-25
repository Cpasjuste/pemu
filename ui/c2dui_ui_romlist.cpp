//
// Created by cpasjuste on 22/11/16.
//
#include <algorithm>
#include "c2dui.h"
#include "ss_api.h"

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

UIRomList::UIRomList(UiMain *u, RomList *rList, const c2d::Vector2f &size)
        : SkinnedRectangle(u->getSkin(), {"MAIN"}) {
    printf("UIRomList\n");

    ui = u;
    romList = rList;
    Skin *skin = ui->getSkin();

    // add title image if available
    auto *title = new SkinnedRectangle(ui->getSkin(), {"MAIN", "TITLE"});
    UIRomList::add(title);

    // add help image if available
    auto *help = new SkinnedRectangle(ui->getSkin(), {"MAIN", "HELP"});
    if (help->available) {
        UIRomList::add(help);
    }

    // add rom info ui
    romInfo = new UIRomInfo(ui, this, skin->font, ui->getFontSize());
    UIRomList::add(romInfo);

    // add rom listing ui
    Skin::TextGroup textGroup = skin->getText({"MAIN", "ROM_LIST", "TEXT"});
    config::Group *grp = skin->getConfig()->getGroup("ROM_LIST")->getGroup("TEXT");
    config::Option *opt = grp->getOption("color_missing");
    Color colorMissing;
    if (opt->getType() == config::Option::Type::String) {
        opt = skin->getConfig()->getOption("COLORS", opt->getString());
        if (opt) {
            colorMissing = opt->getColor();
        }
    } else {
        colorMissing = grp->getOption("color_missing")->getColor();
    }

    bool highlightUseFileColors = grp->getOption("highlight_use_text_color")->getInteger() == 1;

    // add rom list title (system text)
    titleText = new SkinnedText(skin, {"MAIN", "ROM_LIST", "SYSTEM_TEXT"});
    if (titleText->available) {
        UIRomList::add(titleText);
    }

    // add rom list ui
    Skin::RectangleShapeGroup romListGroup = skin->getRectangleShape({"MAIN", "ROM_LIST"});
    bool use_icons = false;
#if !(defined(__PSP2__) || defined(__3DS__)) // two slow
    use_icons = ui->getConfig()->get(Option::Id::GUI_SHOW_ICONS)->getValueBool();
#endif
    listBox = new UIListBox(ui, skin->font, (int) textGroup.size,
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
    Skin::RectangleShapeGroup rectShape = skin->getRectangleShape({"SKIN_CONFIG", "HIGHLIGHT"});
    listBox->getHighlight()->setFillColor(rectShape.color);
    listBox->getHighlight()->setOutlineColor(rectShape.outlineColor);
    listBox->getHighlight()->setOutlineThickness((float) rectShape.outlineSize);
    listBox->setHighlightUseFileColor(highlightUseFileColors);
    UIRomList::add(listBox);

    // add blur
    blur = new RectangleShape(UIRomList::getLocalBounds());
    blur->setFillColor(Color::Gray);
    blur->add(new TweenAlpha(0, 230, 0.2));
    blur->setVisibility(Visibility::Hidden);
    UIRomList::add(blur);

    int delay = ui->getConfig()->get(Option::Id::GUI_VIDEO_SNAP_DELAY)->getValueInt();
    UIRomList::setVideoSnapDelay(delay);

    // filter roms
    UIRomList::updateRomList();
}

void UIRomList::updateRomList() {
    filterRomList();
    sortRomList();

    if (titleText->available) {
        std::string sys = ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->getValueString();
        titleText->setString(sys);
    }

    if (listBox) {
        listBox->setGames(gameList.games);
    }

    if (romInfo) {
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
            Game parentGame = romList->gameList->findGameByPath(game.cloneOf);
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
            std::vector<Game> clones = romList->gameList->findGamesByName(game);
            for (const auto &g: clones) {
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
        Game parentGame = romList->gameList->findGameByPath(game.cloneOf);
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
            std::vector<Game> clones = romList->gameList->findGamesByName(game);
            for (const auto &g: clones) {
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
    ss_api::GameList *list = opt->getValueString() == "FAVORITES" ? romList->gameListFav : romList->gameList;

    bool available = opt->getValueString() == "FAVORITES" ? false : opt->getValueString() == "AVAILABLE";
    bool showClones = ui->getConfig()->get(Option::Id::GUI_FILTER_CLONES)->getValueBool();
    std::string system = ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->getValueString();
    int systemId;
    // custom arcade system (mame sscrap id 75)
    if (system == "ARCADE") {
        systemId = 9999;
    } else {
        systemId = system == "ALL" ? -1 : list->systemList.findByName(system).id;
    }
    std::string editor = ui->getConfig()->get(Option::Id::GUI_FILTER_EDITOR)->getValueString();
    int editorId = editor == "ALL" ? -1 : list->findEditorByName(editor).id;
    std::string dev = ui->getConfig()->get(Option::Id::GUI_FILTER_DEVELOPER)->getValueString();
    int devId = dev == "ALL" ? -1 : list->findDeveloperByName(dev).id;
    int players = Utility::parseInt(ui->getConfig()->get(Option::Id::GUI_FILTER_PLAYERS)->getValueString(), -1);
    int rating = Utility::parseInt(ui->getConfig()->get(Option::Id::GUI_FILTER_RATING)->getValueString(), -1);
    int rotation = Utility::parseInt(ui->getConfig()->get(Option::Id::GUI_FILTER_ROTATION)->getValueString(), -1);
    std::string genre = ui->getConfig()->get(Option::Id::GUI_FILTER_GENRE)->getValueString();
    int genreId = genre == "ALL" ? -1 : list->findGenreByName(genre).id;

    gameList = list->filter(
            available, showClones, systemId == 9999 ? -1 : systemId, systemId == 9999 ? 75 : -1,
            editorId, devId, players, rating, rotation, genreId,
            ui->getConfig()->get(Option::Id::GUI_FILTER_RESOLUTION)->getValueString(),
            ui->getConfig()->get(Option::Id::GUI_FILTER_DATE)->getValueString()
    );
}

void UIRomList::sortRomList() {
    bool byZipName = ui->getConfig()->get(Option::Id::GUI_SHOW_ZIP_NAMES)->getValueBool();
    gameList.sortAlpha(byZipName);
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

    unsigned int buttons = players[0].buttons;
    if (buttons & Input::Button::Up) {
        listBox->up();
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (buttons & Input::Button::Down) {
        listBox->down();
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (buttons & Input::Button::Right) {
        listBox->setSelection(listBox->getIndex() + listBox->getMaxLines());
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (buttons & Input::Button::Left) {
        listBox->setSelection(listBox->getIndex() - listBox->getMaxLines());
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else if (buttons & Input::Button::A) {
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
    } else if (buttons & Input::Button::X) {
        // add to favorites
        Game game = getSelection();
        if (game.id > 0 && !romList->gameListFav->exist(game.id)) {
            int res = ui->getUiMessageBox()->show(
                    "FAVORITES", "Add to favorites ?", "OK", "CANCEL");
            if (res == MessageBox::LEFT) {
                romList->addFav(game);
            }
        } else if (game.id > 0 && romList->gameListFav->exist(game.id)) {
            int res = ui->getUiMessageBox()->show(
                    "FAVORITES", "Remove from favorites ?", "OK", "CANCEL");
            if (res == MessageBox::LEFT) {
                romList->removeFav(game);
                Option *opt = ui->getConfig()->get(Option::Id::GUI_SHOW_ALL);
                if (opt->getValueString() == "FAVORITES") {
                    // update list if we are in favorites
                    updateRomList();
                }
            }
        }
    } else if (buttons & Input::Button::Menu1) {
        ui->getUiMenu()->load();
    } else if (buttons & Input::Button::Menu2) {
        if (getSelection().id > 0) {
            ui->getUiMenu()->load(true);
        }
    }

    // only allow system switch if skin contains romlist title
    if (titleText->available) {
        if (buttons & Input::Button::LT) {
            Option *sysOpt = ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM);
            size_t sysCount = sysOpt->getValues()->size();
            if (sysCount > 1) {
                sysOpt->prev();
                updateRomList();
            }
        } else if (buttons & Input::Button::RT) {
            Option *sysOpt = ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM);
            size_t sysCount = sysOpt->getValues()->size();
            if (sysCount > 1) {
                sysOpt->next();
                updateRomList();
            }
        }
    }

    return true;
}

void UIRomList::onUpdate() {
    if (!isVisible() || ui->getUiProgressBox()->isVisible()) {
        return;
    }

    unsigned int buttons = ui->getInput()->getButtons();

    if (buttons > 0 && buttons != Input::Delay) {
        timer_load_info.restart();
        timer_load_video.restart();
    } else if (buttons == 0) {
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

    RectangleShape::onUpdate();
}

UIRomList::~UIRomList() {
    printf("~UIRomList\n");
    delete (romList);
}

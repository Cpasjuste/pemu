//
// Created by cpasjuste on 22/11/16.
//
#include <algorithm>
#include "pemu.h"
#include "ss_api.h"

using namespace c2d;
using namespace c2d::config;
using namespace pemu;
using namespace ss_api;

UIRomList::UIRomList(UiMain *main, RomList *romList, const c2d::Vector2f &size)
        : SkinnedRectangle(main, {"SKIN_CONFIG", "MAIN"}) {
    printf("UIRomList\n");
    pMain = main;
    pRomList = romList;
    Skin *skin = pMain->getSkin();

    // add title image if available
    auto *title = new SkinnedRectangle(pMain, {"SKIN_CONFIG", "MAIN", "TITLE"});
    UIRomList::add(title);

    // add help
    auto *help = new UiHelp(pMain);
    UIRomList::add(help);

    // add rom info ui
    pRomInfo = new UIRomInfo(pMain, this, skin->getFont(), pMain->getFontSize());
    UIRomList::add(pRomInfo);

    // add rom list title (system text + rom count)
    pTitleText = new SkinnedText(pMain, {"SKIN_CONFIG", "MAIN", "ROM_LIST", "SYSTEM_TEXT"});
    if (pTitleText->available) {
        UIRomList::add(pTitleText);
    } else {
        delete (pTitleText);
    }
    mShowSystemText = !(pMain->getConfig()->getOption(PEMUConfig::OptId::UI_FILTER_SYSTEM)->getFlags()
                        & PEMUConfig::Flags::HIDDEN);

    // add rom listing ui
    Skin::TextGroup textGroup = skin->getText({"SKIN_CONFIG", "MAIN", "ROM_LIST", "TEXT"});
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

    // add rom list ui
    Skin::RectangleShapeGroup romListGroup = skin->getRectangleShape({"MAIN", "ROM_LIST"});
    pListBox = new UIListBox(pMain, skin->getFont(), (int) textGroup.size,
                             romListGroup.rect, mGameList.games);
    pListBox->colorMissing = colorMissing;
    pListBox->colorAvailable = textGroup.color;
    pListBox->setFillColor(romListGroup.color);
    pListBox->setOutlineColor(romListGroup.outlineColor);
    pListBox->setOutlineThickness((float) romListGroup.outlineSize);
    pListBox->setSelection(0);
    // rom item
    pListBox->setTextOutlineColor(textGroup.outlineColor);
    pListBox->setTextOutlineThickness((float) textGroup.outlineSize);
    // hihglight
    Skin::RectangleShapeGroup rectShape = skin->getRectangleShape({"SKIN_CONFIG", "HIGHLIGHT"});
    pListBox->getHighlight()->setFillColor(rectShape.color);
    pListBox->getHighlight()->setOutlineColor(rectShape.outlineColor);
    pListBox->getHighlight()->setOutlineThickness((float) rectShape.outlineSize);
    pListBox->setHighlightUseFileColor(highlightUseFileColors);
    UIRomList::add(pListBox);

    // add blur
    pBlur = new RectangleShape(UIRomList::getLocalBounds());
    pBlur->setFillColor(Color::Gray);
    pBlur->add(new TweenAlpha(0, 230, 0.2));
    pBlur->setVisibility(Visibility::Hidden);
    UIRomList::add(pBlur);

    int delay = pMain->getConfig()->get(PEMUConfig::OptId::UI_VIDEO_SNAP_DELAY)->getInteger();
    UIRomList::setVideoSnapDelay(delay);
}

std::string UIRomList::getPreview(const Game &game, UIRomList::PreviewType type) {
    std::string mediaType = type == Tex ? "mixrbv2" : "video";
    std::string mediaExt = type == Tex ? ".png" : ".mp4";

    // try media path from database
    std::string path = game.romsPath + game.getMedia(mediaType).url;
    printf("getPreview(%s)\n", path.c_str());
    if (pMain->getIo()->getFile(path).isFile()) {
        return path;
    }

    // try from filename
    path = game.romsPath + "media/" + mediaType + "/" + Utility::removeExt(game.path) + mediaExt;
    printf("getPreview(%s)\n", path.c_str());
    if (pMain->getIo()->getFile(path).isFile()) {
        return path;
    }

    // for non arcade game, search for a "screenscraper" game with same name
    if (!game.isClone()) {
        std::vector<Game> clones = pRomList->gameList->findGamesByName(game);
        for (const auto &g: clones) {
            path = g.romsPath + g.getMedia(mediaType).url;
            printf("getPreview(%s)\n", path.c_str());
            if (pMain->getIo()->getFile(path).isFile()) {
                return path;
            }
        }
    }

    // now try parent image if rom is a clone
    if (game.isClone()) {
        Game parent = pRomList->gameList->findGameByPath(game.cloneOf);
        path = parent.romsPath + parent.getMedia(mediaType).url;
        printf("getPreview(%s)\n", path.c_str());
        if (pMain->getIo()->getFile(path).isFile()) {
            return path;
        }
        // try from filename
        path = parent.romsPath + "media/" + mediaType + "/" + Utility::removeExt(parent.path) + mediaExt;
        printf("getPreview(%s)\n", path.c_str());
        if (pMain->getIo()->getFile(path).isFile()) {
            return path;
        }
    }

    return {};
}

void UIRomList::updateRomList() {
    filterRomList();
    sortRomList();

    std::string sys = pMain->getConfig()->get(PEMUConfig::OptId::UI_FILTER_SYSTEM)->getString();
    int systemId = sys == "ALL" ? -1 : pRomList->gameList->systemList.findByName(sys).id;
    if (mShowSystemText && pTitleText) {
        pTitleText->setString(sys + " - " +
                              std::to_string(mGameList.getAvailableCount()) + "/" +
                              std::to_string(pRomList->gameList->getCount(systemId)));
    } else if (pTitleText) {
        pTitleText->setString(std::to_string(mGameList.getAvailableCount()) + "/" +
                              std::to_string(pRomList->gameList->getCount(systemId)));
    }

    if (pListBox) {
        pListBox->setGames(mGameList.games);
    }

    if (pRomInfo) {
        pRomInfo->load(Game());
        mTimerLoadInfoDone = 0;
        mTimerLoadInfo.restart();
        mTimerLoadVideoDone = 0;
        mTimerLoadVideo.restart();
    }
}

void UIRomList::filterRomList() {
    auto cfg = pMain->getConfig();
    ss_api::GameList *list = cfg->get(PEMUConfig::OptId::UI_FILTER_FAVORITES)->getInteger() ?
                             pRomList->gameListFav : pRomList->gameList;
    std::string system = cfg->get(PEMUConfig::OptId::UI_FILTER_SYSTEM)->getString();
    int systemId = system == "ALL" ? -1 : list->systemList.findByName(system).id;
    std::string editor = cfg->get(PEMUConfig::OptId::UI_FILTER_EDITOR)->getString();
    int editorId = editor == "ALL" ? -1 : list->findEditorByName(editor).id;
    std::string dev = cfg->get(PEMUConfig::OptId::UI_FILTER_DEVELOPER)->getString();
    int devId = dev == "ALL" ? -1 : list->findDeveloperByName(dev).id;
    int players = Utility::parseInt(cfg->get(PEMUConfig::OptId::UI_FILTER_PLAYERS)->getString(), -1);
    int rating = Utility::parseInt(cfg->get(PEMUConfig::OptId::UI_FILTER_RATING)->getString(), -1);
    std::string genre = cfg->get(PEMUConfig::OptId::UI_FILTER_GENRE)->getString();
    int genreId = genre == "ALL" ? -1 : list->findGenreByName(genre).id;

    //printf("UIRomList::filterRomList: system: %s (0x%08x)\n", system.c_str(), systemId);
    mGameList = list->filter(
            cfg->get(PEMUConfig::OptId::UI_FILTER_AVAILABLE)->getInteger(),
            !cfg->get(PEMUConfig::OptId::UI_FILTER_CLONES)->getInteger(),
            systemId, -1, editorId, devId, players, rating, -1, genreId, "ALL",
            cfg->get(PEMUConfig::OptId::UI_FILTER_DATE)->getString()
    );
}

void UIRomList::sortRomList() {
    bool byZipName = pMain->getConfig()->get(PEMUConfig::OptId::UI_SHOW_ZIP_NAMES)->getInteger();
    mGameList.sortAlpha(byZipName);
}

void UIRomList::setVideoSnapDelay(int delay) {
    mTimerLoadVideoDelay = delay * 1000;
}

Game UIRomList::getSelection() {
    return pListBox->getSelection();
}

RomList *UIRomList::getRomList() {
    return pRomList;
}

void UIRomList::setGames(const std::vector<ss_api::Game> &games) {
    pListBox->setGames(games);
}

void UIRomList::setVisibility(c2d::Visibility visibility, bool tweenPlay) {
    if (visibility == c2d::Visibility::Hidden) {
        pRomInfo->load();
        mTimerLoadInfoDone = 0;
        mTimerLoadVideoDone = 0;
    } else {
        mTimerLoadInfo.restart();
        mTimerLoadVideo.restart();
    }

    RectangleShape::setVisibility(visibility, tweenPlay);
}

bool UIRomList::onInput(c2d::Input::Player *players) {
    if (pMain->getUiMenu()->isVisible()
        || pMain->getUiStateMenu()->isVisible()
        || pMain->getUiProgressBox()->isVisible()) {
        return false;
    }

    unsigned int buttons = players[0].buttons;
    if (buttons & Input::Button::Up) {
        pListBox->up();
        pRomInfo->load();
        mTimerLoadInfoDone = 0;
        mTimerLoadVideoDone = 0;
    } else if (buttons & Input::Button::Down) {
        pListBox->down();
        pRomInfo->load();
        mTimerLoadInfoDone = 0;
        mTimerLoadVideoDone = 0;
    } else if (buttons & Input::Button::Right) {
        pListBox->setSelection(pListBox->getIndex() + pListBox->getMaxLines());
        pRomInfo->load();
        mTimerLoadInfoDone = 0;
        mTimerLoadVideoDone = 0;
    } else if (buttons & Input::Button::Left) {
        pListBox->setSelection(pListBox->getIndex() - pListBox->getMaxLines());
        pRomInfo->load();
        mTimerLoadInfoDone = 0;
        mTimerLoadVideoDone = 0;
    } else if (buttons & Input::Button::A) {
        Game game = getSelection();
        if (game.available) {
#ifdef __MPV__
            pRomInfo->mpvTexture->setVisibility(c2d::Visibility::Hidden);
            pRomInfo->mpv->stop();
#endif
            pMain->getConfig()->loadGame(game);
            pMain->getUiEmu()->load(game);
            return true;
        }
    } else if (buttons & Input::Button::X) {
        // add to favorites
        Game game = getSelection();
        if (game.id > 0 && !pRomList->gameListFav->exist(game.id)) {
            int res = pMain->getUiMessageBox()->show(
                    "FAVORITES", "Add to favorites ?", "OK", "CANCEL");
            if (res == MessageBox::LEFT) {
                pRomList->addFav(game);
            }
        } else if (game.id > 0 && pRomList->gameListFav->exist(game.id)) {
            int res = pMain->getUiMessageBox()->show(
                    "FAVORITES", "Remove from favorites ?", "OK", "CANCEL");
            if (res == MessageBox::LEFT) {
                pRomList->removeFav(game);
                if (pMain->getConfig()->get(PEMUConfig::OptId::UI_FILTER_FAVORITES)->getInteger()) {
                    // update list if we are in favorites
                    updateRomList();
                }
            }
        }
    } else if (buttons & Input::Button::Menu1) {
        pMain->getUiMenu()->load();
    } else if (buttons & Input::Button::Menu2) {
        if (getSelection().id > 0) {
            pMain->getUiMenu()->load(true);
        }
    }

    // only allow system switch if skin contains romlist title
    if (pTitleText && mShowSystemText) {
        if (buttons & Input::Button::LT) {
            Option *sysOpt = pMain->getConfig()->get(PEMUConfig::OptId::UI_FILTER_SYSTEM);
            size_t sysCount = sysOpt->getArray().size();
            if (sysCount > 1) {
                sysOpt->setArrayMovePrev();
                updateRomList();
            }
        } else if (buttons & Input::Button::RT) {
            Option *sysOpt = pMain->getConfig()->get(PEMUConfig::OptId::UI_FILTER_SYSTEM);
            size_t sysCount = sysOpt->getArray().size();
            if (sysCount > 1) {
                sysOpt->setArrayMoveNext();
                updateRomList();
            }
        }
    }

    return true;
}

void UIRomList::onUpdate() {
    if (!isVisible() || pMain->getUiProgressBox()->isVisible()) {
        return;
    }

    unsigned int buttons = pMain->getInput()->getButtons();

    if (buttons > 0 && buttons != Input::Delay) {
        mTimerLoadInfo.restart();
        mTimerLoadVideo.restart();
    } else {
        if ((mTimerLoadInfoDone == 0) && mTimerLoadInfo.getElapsedTime().asMilliseconds() > mTimerLoadInfoDelay) {
            pRomInfo->load(pListBox->getSelection());
            mTimerLoadInfoDone = 1;
        }
        if (mTimerLoadVideoDelay > 0 && mTimerLoadVideoDone == 0 &&
            mTimerLoadVideo.getElapsedTime().asMilliseconds() > mTimerLoadVideoDelay) {
            pRomInfo->loadVideo(pListBox->getSelection());
            mTimerLoadVideoDone = 1;
        }
    }

    RectangleShape::onUpdate();
}

UIRomList::~UIRomList() {
    printf("~UIRomList\n");
    delete (pRomList);
}

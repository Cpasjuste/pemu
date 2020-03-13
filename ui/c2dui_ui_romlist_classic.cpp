//
// Created by cpasjuste on 22/11/16.
//
#include <algorithm>
#include "c2dui.h"
#include "ss_api.h"

#ifdef __MPV__

#include "mpv.h"
#include "mpv_texture.h"
#include "c2dui_ui_romlist_classic.h"


#endif

/// pFBA
#define BDF_ORIENTATION_FLIPPED     (1 << 1)
#define BDF_ORIENTATION_VERTICAL    (1 << 2)

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

class UIRomInfo : public Rectangle {

public:

    UIRomInfo(UIMain *u, UIRomList *uiRList, Font *fnt, int fntSize)
            : Rectangle(u->getSize()) {

        printf("UIRomInfo\n");

        ui = u;
        uiRomList = uiRList;
        font = fnt;
        fontSize = fntSize;

        // synopsis box
        synoBox = new RectangleShape({16, 16});
        ui->getSkin()->loadRectangleShape(synoBox, {"MAIN", "ROM_SYNOPSIS"});
        synoText = new Text("", (unsigned int) fontSize, font);
        ui->getSkin()->loadText(synoText, {"MAIN", "ROM_SYNOPSIS", "TEXT"});
        synoBox->add(synoText);
        add(synoBox);

        // info box
        infoBox = new RectangleShape({16, 16});
        ui->getSkin()->loadRectangleShape(infoBox, {"MAIN", "ROM_INFOS"});
        // info box texts
        systemText = addInfoBoxText({"MAIN", "ROM_INFOS", "SYSTEM_TEXT"});
        developerText = addInfoBoxText({"MAIN", "ROM_INFOS", "DEVELOPER_TEXT"});
        editorText = addInfoBoxText({"MAIN", "ROM_INFOS", "EDITOR_TEXT"});
        dateText = addInfoBoxText({"MAIN", "ROM_INFOS", "DATE_TEXT"});
        genreText = addInfoBoxText({"MAIN", "ROM_INFOS", "GENRE_TEXT"});
        playersText = addInfoBoxText({"MAIN", "ROM_INFOS", "PLAYERS_TEXT"});
        ratingText = addInfoBoxText({"MAIN", "ROM_INFOS", "RATING_TEXT"});
        topstaffText = addInfoBoxText({"MAIN", "ROM_INFOS", "TOPSTAFF_TEXT"});
        rotationText = addInfoBoxText({"MAIN", "ROM_INFOS", "ROTATION_TEXT"});
        resolutionText = addInfoBoxText({"MAIN", "ROM_INFOS", "RESOLUTION_TEXT"});
        classificationText = addInfoBoxText({"MAIN", "ROM_INFOS", "CLASSIFICATION_TEXT"});
        cloneofText = addInfoBoxText({"MAIN", "ROM_INFOS", "CLONEOF_TEXT"});
        filenameText = addInfoBoxText({"MAIN", "ROM_INFOS", "FILENAME_TEXT"});
        add(infoBox);

        // preview box
        previewBox = new RectangleShape(FloatRect(0, 0, getSize().x, getSize().y / 2));
        ui->getSkin()->loadRectangleShape(previewBox, {"MAIN", "ROM_IMAGE"});
        previewText = new Text("", (unsigned int) fontSize, font);
        ui->getSkin()->loadText(previewText, {"MAIN", "ROM_IMAGE", "TEXT"});
        previewBox->add(previewText);
        add(previewBox);

#ifdef __MPV__
        mpv = new Mpv(ui->getIo()->getDataPath() + "mpv", true);
        mpvTexture = new MpvTexture(previewBox->getSize(), mpv);
        mpvTexture->setOrigin(previewBox->getOrigin());
        mpvTexture->setPosition(previewBox->getPosition());
        add(mpvTexture);
#endif
    }

    ~UIRomInfo() override {
        printf("~UIRomInfo\n");
#ifdef __MPV__
        delete (mpv);
#endif
    }

    Text *addInfoBoxText(const std::vector<std::string> &tree) {
        Text *text = new Text("", (unsigned int) fontSize, font);
        ui->getSkin()->loadText(text, tree);
        infoBox->add(text);
        return text;
    }

    void showText(Text *text, const std::string &msg) {
        text->setString(msg);
        text->setVisibility(Visibility::Visible);
    }

    void hideText(Text *text) {
        text->setVisibility(Visibility::Hidden);
        text->setString("");
    }

    bool loadVideo(const Game &game) {
#ifdef __MPV__
        std::string videoPath = uiRomList->getPreviewVideo(game);
        if (!videoPath.empty()) {
            int res = mpv->load(videoPath, Mpv::LoadType::Replace, "loop=yes");
            if (res == 0) {
                mpvTexture->setLayer(1);
                mpvTexture->setVisibility(Visibility::Visible, false);
            }
        }
#endif
        return true;
    }

    bool loadTexture(const Game &game) {
        texture = (C2DTexture *) uiRomList->getPreviewTexture(game);
        // set image
        if ((texture != nullptr) && texture->available) {
            previewText->setVisibility(Visibility::Hidden);
            texture->setOrigin(Origin::Center);
            texture->setPosition(Vector2f(previewBox->getSize().x / 2, previewBox->getSize().y / 2));
            float tex_scaling = std::min(
                    previewBox->getSize().x / texture->getTextureRect().width,
                    previewBox->getSize().y / texture->getTextureRect().height);
            texture->setScale(tex_scaling, tex_scaling);
            previewBox->add(texture);
        } else {
            previewText->setVisibility(Visibility::Visible);
            return false;
        }

        return true;
    }

    void load(const Game &game = Game()) {

        if (texture != nullptr) {
            delete (texture);
            texture = nullptr;
        }

        if (game.id <= 0) {
            hideText(systemText);
            hideText(developerText);
            hideText(editorText);
            hideText(dateText);
            hideText(genreText);
            hideText(playersText);
            hideText(ratingText);
            hideText(topstaffText);
            hideText(rotationText);
            hideText(resolutionText);
            hideText(classificationText);
            hideText(cloneofText);
            hideText(filenameText);
            hideText(synoText);
#if __MPV__
            mpvTexture->setVisibility(Visibility::Hidden);
            mpv->stop();
#endif
        } else {
            printf("load(%s)\n", game.getName().text.c_str());
            // load title/preview texture
            loadTexture(game);
            showText(systemText, "System: " + game.system.text);
            showText(developerText, "Developer: " + game.developer.text);
            showText(editorText, "Editor: " + game.editor.text);
            showText(dateText, "Date: " + game.getDate().text);
            showText(genreText, "Genre: " + game.getGenre().text);
            showText(playersText, "Players: " + game.players);
            showText(ratingText, "Rating: " + std::to_string(game.rating));
            showText(topstaffText, "Top Staff: " + std::to_string((int) game.topStaff));
            showText(rotationText, "Rotation: " + std::to_string(game.rotation));
            showText(resolutionText, "Resolution: " + game.resolution);
            // TODO:
            //showText(classificationText, "Classification: " + game.getClassification().text);
            showText(classificationText, "Classification: Unknown");
            showText(cloneofText, "Clone Of: " + game.cloneOf);
            showText(filenameText, "File: " + game.path);
            //
            showText(synoText, game.getSynopsis().text);
        }
    }

    UIMain *ui = nullptr;
    UIRomList *uiRomList = nullptr;
    C2DTexture *texture = nullptr;
    Font *font;
    int fontSize;
    //
    RectangleShape *synoBox = nullptr;
    Text *synoText = nullptr;
    //
    RectangleShape *previewBox = nullptr;
    Text *previewText = nullptr;
    //
    RectangleShape *infoBox = nullptr;
    Text *systemText = nullptr;
    Text *developerText = nullptr;
    Text *editorText = nullptr;
    Text *dateText = nullptr;
    Text *genreText = nullptr;
    Text *playersText = nullptr;
    Text *ratingText = nullptr;
    Text *topstaffText = nullptr;
    Text *rotationText = nullptr;
    Text *resolutionText = nullptr;
    Text *classificationText = nullptr;
    Text *cloneofText = nullptr;
    Text *filenameText = nullptr;
#ifdef __MPV__
    Mpv *mpv = nullptr;
    MpvTexture *mpvTexture = nullptr;
#endif
};

UIRomListClassic::UIRomListClassic(UIMain *u, RomList *romList, const c2d::Vector2f &size)
        : UIRomList(u, romList, size) {

    printf("UIRomListClassic\n");

    Skin *skin = ui->getSkin();

    // set gui main "window"
    skin->loadRectangleShape(this, {"MAIN"});

    // add title image if available
    auto title = new RectangleShape({16, 16});
    skin->loadRectangleShape(title, {"MAIN", "TITLE"});
    add(title);

    // add help image if available
    auto help = new RectangleShape({16, 16});
    skin->loadRectangleShape(help, {"MAIN", "HELP"});
    add(help);

    // add rom info ui
    romInfo = new UIRomInfo(ui, this, skin->font, ui->getFontSize());
    add(romInfo);

    int delay = ui->getConfig()->get(Option::Id::GUI_VIDEO_SNAP_DELAY)->getValueInt();
    setVideoSnapDelay(delay);

    // filter roms
    updateRomList();
}

Game UIRomListClassic::getSelection() {
    return listBox->getSelection();
}

void UIRomListClassic::updateRomList() {

    filterRomList();

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
        listBox = new UIListBox(ui->getSkin()->font, (int) textGroup.size, romListGroup.rect, gameList.games,
                                use_icons);
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

void UIRomListClassic::setVisibility(c2d::Visibility visibility, bool tweenPlay) {

    if (visibility == c2d::Visibility::Hidden) {
        romInfo->load();
        timer_load_info_done = 0;
        timer_load_video_done = 0;
    } else {
        timer_load_info.restart();
        timer_load_video.restart();
    }

    UIRomList::setVisibility(visibility, tweenPlay);
}

bool UIRomListClassic::onInput(c2d::Input::Player *players) {

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
    } else if (keys & Input::Key::Fire4) {
        // remove from favorites
        Game game = getSelection();
        if (game.id > 0 && romList->gameListFav.exist(game.romId)) {
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

void UIRomListClassic::onUpdate() {

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

UIRomListClassic::~UIRomListClassic() {
    printf("~UIRomListClassic\n");
}

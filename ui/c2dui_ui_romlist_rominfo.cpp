//
// Created by cpasjuste on 12/05/2020.
//

#include "c2dui.h"

UIRomInfo::UIRomInfo(UiMain *u, UIRomList *uiRList, Font *fnt, int fntSize)
        : Rectangle(u->getSize()) {

    printf("UIRomInfo\n");

    ui = u;
    uiRomList = uiRList;
    font = fnt;
    fontSize = fntSize;

    // synopsis box
    synoBox = new RectangleShape({16, 16});
    if (ui->getSkin()->loadRectangleShape(synoBox, {"MAIN", "ROM_SYNOPSIS"})) {
        synoText = new Text("", (unsigned int) fontSize, font);
        ui->getSkin()->loadText(synoText, {"MAIN", "ROM_SYNOPSIS", "TEXT"});
        synoBox->add(synoText);
        Rectangle::add(synoBox);
    } else {
        delete (synoBox);
    }

    // info box
    infoBox = new RectangleShape({16, 16});
    if (ui->getSkin()->loadRectangleShape(infoBox, {"MAIN", "ROM_INFOS"})) {
        systemText = addInfoBoxText({"MAIN", "ROM_INFOS", "SYSTEM_TEXT"});
        developerText = addInfoBoxText({"MAIN", "ROM_INFOS", "DEVELOPER_TEXT"});
        editorText = addInfoBoxText({"MAIN", "ROM_INFOS", "EDITOR_TEXT"});
        dateText = addInfoBoxText({"MAIN", "ROM_INFOS", "DATE_TEXT"});
        genreText = addInfoBoxText({"MAIN", "ROM_INFOS", "GENRE_TEXT"});
        playersText = addInfoBoxText({"MAIN", "ROM_INFOS", "PLAYERS_TEXT"});
        ratingText = addInfoBoxText({"MAIN", "ROM_INFOS", "RATING_TEXT"});
        rotationText = addInfoBoxText({"MAIN", "ROM_INFOS", "ROTATION_TEXT"});
        resolutionText = addInfoBoxText({"MAIN", "ROM_INFOS", "RESOLUTION_TEXT"});
        cloneofText = addInfoBoxText({"MAIN", "ROM_INFOS", "CLONEOF_TEXT"});
        filenameText = addInfoBoxText({"MAIN", "ROM_INFOS", "FILENAME_TEXT"});
        Rectangle::add(infoBox);
    } else {
        delete (infoBox);
    }

    // preview box
    previewBox = new RectangleShape(FloatRect(0, 0,
                                              Rectangle::getSize().x, Rectangle::getSize().y / 2));
    ui->getSkin()->loadRectangleShape(previewBox, {"MAIN", "ROM_IMAGE"}, true);
    Rectangle::add(previewBox);

#ifdef __MPV__
    mpv = new Mpv(ui->getIo()->getDataPath() + "mpv", true);
    mpvTexture = new MpvTexture({previewBox->getSize().x, previewBox->getSize().y}, mpv);
    mpvTexture->setOrigin(previewBox->getOrigin());
    mpvTexture->setPosition(previewBox->getPosition());
    mpvTexture->setAlpha(0);
    mpvTexture->setVisibility(Visibility::Hidden);
    mpvTexture->add(new TweenAlpha(0, 255, 0.3f, TweenLoop::None, TweenState::Playing));
    Rectangle::add(mpvTexture);
#endif
}

Text *UIRomInfo::addInfoBoxText(const std::vector<std::string> &tree) {
    Text *text = new Text("", (unsigned int) fontSize, font);
    if (ui->getSkin()->loadText(text, tree)) {
        infoBox->add(text);
    } else {
        delete text;
        text = nullptr;
    }
    return text;
}

void UIRomInfo::showText(Text *text, const std::string &msg) {
    if (text != nullptr) {
        text->setString(msg);
        text->setVisibility(Visibility::Visible);
    }
}

void UIRomInfo::hideText(Text *text) {
    if (text != nullptr) {
        text->setVisibility(Visibility::Hidden);
        text->setString("");
    }
}

bool UIRomInfo::loadVideo(const Game &game) {
#ifdef __MPV__
    int res = -1;
    std::string videoPath = uiRomList->getPreviewVideo(game);
    if (!videoPath.empty()) {
        res = mpv->load(videoPath, Mpv::LoadType::Replace, "loop=yes");
        if (res == 0) {
            mpvTexture->setLayer(1);
            mpvTexture->setVisibility(Visibility::Visible, true);
            if (texture != nullptr) {
                texture->setVisibility(Visibility::Hidden, true);
            }
        }
    }
    if (res != 0) {
        if (mpvTexture->isVisible()) {
            mpvTexture->setVisibility(Visibility::Hidden, true);
        }
        mpv->stop();
    }
#endif
    return true;
}

bool UIRomInfo::loadTexture(const Game &game) {

    auto *tex = game.path.empty() ? nullptr : (C2DTexture *) uiRomList->getPreviewTexture(game);
    // set image
    if (tex) {
        if (texture) {
            delete (texture);
        }
        texture = tex;
        texture->setOrigin(Origin::Center);
        texture->setPosition(Vector2f(previewBox->getSize().x / 2, previewBox->getSize().y / 2));
        float tex_scaling = std::min(
                previewBox->getSize().x / texture->getTextureRect().width,
                previewBox->getSize().y / texture->getTextureRect().height);
        texture->setScale(tex_scaling, tex_scaling);
        texture->setAlpha(0);
        texture->add(new TweenAlpha(0, 255, 0.3f, TweenLoop::None, TweenState::Playing));
        previewBox->add(texture);
    } else {
        if (texture && texture->isVisible()) {
            texture->setVisibility(Visibility::Hidden, true);
        }
        return false;
    }

    return true;
}

void UIRomInfo::load(const Game &game) {

    if (game.id <= 0) {
        // try to load title/preview texture even if game is not in DB
        loadTexture(game);
        hideText(systemText);
        hideText(developerText);
        hideText(editorText);
        hideText(dateText);
        hideText(genreText);
        hideText(playersText);
        hideText(ratingText);
        hideText(rotationText);
        hideText(resolutionText);
        hideText(cloneofText);
        hideText(filenameText);
        hideText(synoText);
#if __MPV__
        mpvTexture->setVisibility(Visibility::Hidden, true);
        mpv->stop();
#endif
    } else {
        printf("UIRomInfo::load(%s)\n", game.name.c_str());
        // load title/preview texture
        loadTexture(game);
        showText(systemText, "System: " + game.system.name);
        showText(developerText, "Developer: " + game.developer.name);
        showText(editorText, "Editor: " + game.editor.name);
        showText(dateText, "Date: " + game.date);
        showText(genreText, "Genre: " + game.genre.name);
        showText(playersText, "Players: " + game.players);
        showText(ratingText, "Rating: " + std::to_string(game.rating));
        showText(rotationText, "Rotation: " + std::to_string(game.rotation));
        showText(resolutionText, "Resolution: " + (game.resolution.empty() ? "UNKNOWN" : game.resolution));
        showText(cloneofText, "Clone Of: " + (game.cloneOf.empty() ? "NONE" : game.cloneOf));
        showText(filenameText, "File: " + game.path);
        showText(synoText, game.synopsis);
    }
}

UIRomInfo::~UIRomInfo() {
    printf("~UIRomInfo\n");
#ifdef __MPV__
    delete (mpv);
#endif
}

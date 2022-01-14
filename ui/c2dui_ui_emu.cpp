//
// Created by cpasjuste on 03/02/18.
//

#include "c2dui.h"

UIEmu::UIEmu(UiMain *u) : RectangleShape(u->getSize()) {

    printf("UIEmu()\n");

    ui = u;
    RectangleShape::setFillColor(Color::Transparent);

    fpsText = new Text("0123456789", (unsigned int) ui->getFontSize(), ui->getSkin()->font);
    fpsText->setString("FPS: 00/60");
    fpsText->setPosition(16, 16);
    fpsText->setVisibility(Visibility::Hidden);
    RectangleShape::add(fpsText);

    RectangleShape::setVisibility(Visibility::Hidden);
}

bool UIEmu::onInput(c2d::Input::Player *players) {

    if (ui->getUiMenu()->isVisible() || ui->getUiStateMenu()->isVisible()) {
        return C2DObject::onInput(players);
    }

    // look for player 1 menu combo
    if (((players[0].keys & Input::Key::Menu1) && (players[0].keys & Input::Key::Menu2))) {
        pause();
        ui->getUiMenu()->load(true);
        ui->getInput()->clear();
        return true;
    }

    // look for window resize event
    if (players[0].keys & EV_RESIZE) {
        // useful for sdl resize event
        getVideo()->updateScaling();
    }

    return C2DObject::onInput(players);
}

void UIEmu::addAudio(c2d::Audio *_audio) {

    if (audio != nullptr) {
        delete (audio);
        audio = nullptr;
    }

    audio = _audio;
}

void UIEmu::addAudio(int rate, int samples, Audio::C2DAudioCallback cb) {

    if (audio != nullptr) {
        delete (audio);
        audio = nullptr;
    }

    auto *aud = new C2DAudio(rate, samples, cb);
    addAudio(aud);
}

void UIEmu::addVideo(C2DUIVideo *_video) {

    if (video != nullptr) {
        delete (video);
        video = nullptr;
    }

    video = _video;
    video->setShader(ui->getConfig()->get(Option::Id::ROM_SHADER, true)->getIndex());
    video->setFilter((Texture::Filter) ui->getConfig()->get(Option::Id::ROM_FILTER, true)->getIndex());
    video->updateScaling();
    add(video);
}

void UIEmu::addVideo(void **pixels, int *pitch,
                     const c2d::Vector2f &size, Texture::Format format) {

    if (video != nullptr) {
        delete (video);
        video = nullptr;
    }

    auto *_video = new C2DUIVideo(ui, pixels, pitch, size, format);
    _video->setShader(ui->getConfig()->get(Option::Id::ROM_SHADER, true)->getIndex());
    _video->setFilter((Texture::Filter) ui->getConfig()->get(Option::Id::ROM_FILTER, true)->getIndex());
    _video->updateScaling();
    addVideo(_video);
}

int UIEmu::load(const Game &game) {

    printf("UIEmu::load(%s)\n", game.path.c_str());
    currentGame = game;

    // set fps text on top
    getFpsText()->setLayer(2);

    setVisibility(Visibility::Visible);
    ui->getUiProgressBox()->setVisibility(Visibility::Hidden);
    ui->getUiRomList()->setVisibility(Visibility::Hidden);

    resume();

    return 0;
}

void UIEmu::pause() {

    printf("UIEmu::pause()\n");

    if (audio != nullptr) {
        audio->pause(1);
    }
    // set ui input configuration
    ui->updateInputMapping(false);
    // enable auto repeat delay
    ui->getInput()->setRepeatDelay(INPUT_DELAY);

    paused = true;
}

void UIEmu::resume() {

    printf("UIEmu::resume()\n");

    // set per rom input configuration
    ui->updateInputMapping(true);
    // disable auto repeat delay
    ui->getInput()->setRepeatDelay(0);

    if (audio != nullptr) {
        audio->pause(0);
    }

    paused = false;
}

void UIEmu::stop() {

    printf("UIEmu::stop()\n");

    if (audio != nullptr) {
        printf("Closing audio...\n");
        audio->pause(1);
        delete (audio);
        audio = nullptr;
    }

    if (video != nullptr) {
        printf("Closing video...\n");
        delete (video);
        video = nullptr;
    }

    //ui->getUiHighlight()->setVisibility(Visibility::Visible);
    ui->updateInputMapping(false);
    setVisibility(Visibility::Hidden);
}

void UIEmu::onUpdate() {

    C2DObject::onUpdate();

    if (!isPaused()) {
        // fps
        bool showFps = ui->getConfig()->get(Option::Id::ROM_SHOW_FPS, true)->getValueBool();
        if (showFps) {
            if (!fpsText->isVisible()) {
                fpsText->setVisibility(c2d::Visibility::Visible);
            }
            sprintf(fpsString, "FPS: %.3g/%i", ui->getFps(), (int) targetFps);
            fpsText->setString(getFpsString());
        } else {
            if (fpsText->isVisible()) {
                fpsText->setVisibility(c2d::Visibility::Hidden);
            }
        }
    }
}

UiMain *UIEmu::getUi() {
    return ui;
}

C2DUIVideo *UIEmu::getVideo() {
    return video;
}

c2d::Audio *UIEmu::getAudio() {
    return audio;
}

float UIEmu::getFrameDuration() {
    return frameDuration;
}

void UIEmu::setFrameDuration(float f) {
    frameDuration = f;
}

c2d::Text *UIEmu::getFpsText() {
    return fpsText;
}

char *UIEmu::getFpsString() {
    return fpsString;
}

bool UIEmu::isPaused() {
    return paused;
}

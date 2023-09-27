//
// Created by cpasjuste on 03/02/18.
//

#include "pemu.h"

UiEmu::UiEmu(UiMain *u) : RectangleShape(u->getSize()) {
    printf("UiEmu()\n");

    pMain = u;
    RectangleShape::setFillColor(Color::Transparent);

    fpsText = new Text("0123456789", (unsigned int) pMain->getFontSize(), pMain->getSkin()->getFont());
    fpsText->setFillColor(Color::Yellow);
    fpsText->setOutlineColor(Color::Black);
    fpsText->setOutlineThickness(1);
    fpsText->setString("FPS: 00/60");
    fpsText->setPosition(16, 16);
    fpsText->setVisibility(Visibility::Hidden);
    RectangleShape::add(fpsText);

    RectangleShape::setVisibility(Visibility::Hidden);
}

void UiEmu::addAudio(c2d::Audio *_audio) {
    delete (audio);
    audio = _audio;
}

void UiEmu::addAudio(int rate, int samples, Audio::C2DAudioCallback cb) {
    auto *aud = new C2DAudio(rate, samples, cb);
    addAudio(aud);
}

void UiEmu::addVideo(C2DUIVideo *v) {
    delete (video);
    video = v;
    video->setShader(pMain->getConfig()->get(PEMUConfig::OptId::EMU_SHADER, true)->getArrayIndex());
    video->setFilter((Texture::Filter) pMain->getConfig()->get(PEMUConfig::OptId::EMU_FILTER, true)->getArrayIndex());
    video->updateScaling();
    add(video);
}

void UiEmu::addVideo(uint8_t **pixels, int *pitch,
                     const c2d::Vector2i &size, const c2d::Vector2i &aspect, Texture::Format format) {
    auto *v = new C2DUIVideo(pMain, pixels, pitch, size, aspect, format);
    addVideo(v);
}

int UiEmu::load(const Game &game) {
    printf("UiEmu::load: name: %s, path: %s\n",
           game.path.c_str(), game.romsPath.c_str());
    pMain->getUiStatusBox()->show("TIPS: PRESS MENU1 + MENU2 BUTTONS FOR IN GAME MENU...");
    currentGame = game;

    // set fps text on top
    getFpsText()->setLayer(2);

    setVisibility(Visibility::Visible);
    pMain->getUiProgressBox()->setVisibility(Visibility::Hidden);
    pMain->getUiRomList()->setVisibility(Visibility::Hidden);
    pMain->getUiRomList()->getBlur()->setVisibility(Visibility::Hidden);

    resume();

    return 0;
}

void UiEmu::pause() {
    printf("UiEmu::pause()\n");
    if (audio != nullptr) {
        audio->pause(1);
    }
    // set ui input configuration
    pMain->updateInputMapping(false);
    // enable auto repeat delay
    pMain->getInput()->setRepeatDelay(INPUT_DELAY);

    paused = true;
}

void UiEmu::resume() {
    printf("UiEmu::resume()\n");
    // set per rom input configuration
    pMain->updateInputMapping(true);
    // disable auto repeat delay
    pMain->getInput()->setRepeatDelay(0);

#ifdef __VITA__
    config::Option *option = pMain->getConfig()->get(PEMUConfig::Id::ROM_WAIT_RENDERING, true);
    if (option) {
        ((PSP2Renderer *) pMain)->setWaitRendering(option->getInteger());
    }
#endif

    if (audio) {
        audio->pause(0);
    }

    paused = false;
}

void UiEmu::stop() {
    printf("UiEmu::stop()\n");
    if (audio) {
        printf("Closing audio...\n");
        audio->pause(1);
        delete (audio);
        audio = nullptr;
    }

    if (video) {
        printf("Closing video...\n");
        delete (video);
        video = nullptr;
    }

#ifdef __VITA__
    ((PSP2Renderer *) pMain)->setWaitRendering(true);
#endif

    pMain->updateInputMapping(false);
    setVisibility(Visibility::Hidden);
}

bool UiEmu::onInput(c2d::Input::Player *players) {
    if (pMain->getUiMenu()->isVisible() || pMain->getUiStateMenu()->isVisible()) {
        return C2DObject::onInput(players);
    }

    // look for player 1 menu combo
    if (((players[0].buttons & Input::Button::Menu1) && (players[0].buttons & Input::Button::Menu2))) {
        pause();
        pMain->getUiMenu()->load(true);
        pMain->getInput()->clear();
        return true;
    }

    return C2DObject::onInput(players);
}

void UiEmu::onUpdate() {
    C2DObject::onUpdate();

    if (!isPaused()) {
        // fps
        bool showFps = pMain->getConfig()->get(PEMUConfig::OptId::EMU_SHOW_FPS, true)->getInteger();
        if (showFps) {
            if (!fpsText->isVisible()) {
                fpsText->setVisibility(c2d::Visibility::Visible);
            }
            sprintf(fpsString, "FPS: %.1f/%.1f", pMain->getFps(), targetFps);
            fpsText->setString(fpsString);
        } else {
            if (fpsText->isVisible()) {
                fpsText->setVisibility(c2d::Visibility::Hidden);
            }
        }
    }
}

UiMain *UiEmu::getUi() {
    return pMain;
}

C2DUIVideo *UiEmu::getVideo() {
    return video;
}

c2d::Audio *UiEmu::getAudio() {
    return audio;
}

c2d::Text *UiEmu::getFpsText() {
    return fpsText;
}

bool UiEmu::isPaused() {
    return paused;
}

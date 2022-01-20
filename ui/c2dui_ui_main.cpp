//
// Created by cpasjuste on 22/11/16.
//

#include <algorithm>
#include "c2dui.h"

UiMain::UiMain(const Vector2f &size, c2d::Io *io, Config *cfg) : C2DRenderer(size) {
    printf("UiMain(%ix%i)\n", (int) UiMain::getSize().x, (int) UiMain::getSize().y);

    setIo(io);
    setConfig(cfg);

    // add shaders, if any
    if (getShaderList() != nullptr) {
        config->add(Option::Id::ROM_FILTER, "EFFECT", getShaderList()->getNames(), 0,
                    Option::Id::ROM_SHADER, Option::Flags::STRING);
    } else {
        config->add(Option::Id::ROM_FILTER, "EFFECT", {"NONE"}, 0,
                    Option::Id::ROM_SHADER, Option::Flags::STRING | Option::Flags::HIDDEN);
    }
}

UiMain::~UiMain() {
    // ui elements (C2DObject)
    // are deleted by the renderer
#if 0
    delete (scrapper);
#endif
}

void UiMain::init(UIRomList *_uiRomList, UiMenu *_uiMenu,
                  UIEmu *_uiEmu, UiStateMenu *_uiState) {
#if 0
    uiHighlight = new UIHighlight();
    skin->loadRectangleShape(uiHighlight, {"SKIN_CONFIG", "HIGHLIGHT"});
    float alpha = uiHighlight->getAlpha();
    if (alpha > 0) {
        uiHighlight->add(new TweenAlpha((float) uiHighlight->getAlpha() * 0.5f,
                                        uiHighlight->getAlpha(), 0.5f, TweenLoop::PingPong));
    }
    uiHighlight->setOrigin(Origin::Center);
    uiHighlight->setLayer(1);
    uiHighlight->setVisibility(Visibility::Hidden);
    add(uiHighlight);
#endif

    uiRomList = _uiRomList;
    uiRomList->updateRomList();
    add(uiRomList);

    // build menus from options
    uiMenu = _uiMenu;
    add(uiMenu);

    uiEmu = _uiEmu;
    add(uiEmu);

    uiState = _uiState;
    add(uiState);

    // scaling factor mainly used for borders,
    scaling = std::min(getSize().x / 640, 1.0f);
    // printf("scaling: %f\n", scaling);

    uiMessageBox = new c2d::MessageBox(
            FloatRect(
                    getSize().x / 2,
                    getSize().y / 2,
                    getSize().x / 2,
                    getSize().y / 2),
            getInput(), skin->font, getFontSize());
    skin->loadRectangleShape(uiMessageBox, {"SKIN_CONFIG", "MESSAGEBOX"});
    uiMessageBox->getTitleText()->setSize(
            uiMessageBox->getTitleText()->getSize().x * 1.5f, uiMessageBox->getTitleText()->getSize().y * 1.5f);
    uiMessageBox->setSelectedColor(uiMessageBox->getFillColor(), uiMessageBox->getOutlineColor());
    Color c = uiMessageBox->getOutlineColor();
    c.a -= 150;
    uiMessageBox->setNotSelectedColor(uiMessageBox->getFillColor(), c);
    uiMessageBox->setOrigin(Origin::Center);
#if C2D_SCREEN_WIDTH <= 400
    uiMessageBox->getTitleText()->setOutlineThickness(1);
    uiMessageBox->getMessageText()->setOutlineThickness(1);
#endif
    add(uiMessageBox);

    uiProgressBox = new UIProgressBox(this);
    add(uiProgressBox);

    updateInputMapping(false);
    getInput()->setRepeatDelay(INPUT_DELAY);

#if 0
    scrapper = new Scrapper(this);
#endif
}

void UiMain::setSkin(Skin *s) {
    skin = s;
}

void UiMain::onUpdate() {

    unsigned int keys = getInput()->getKeys(0);
    if (keys & EV_QUIT) {
        done = true;
        return C2DRenderer::onUpdate();
    }

    if (uiEmu && !uiEmu->isVisible()) {
        if (keys != Input::Key::Delay) {
            bool changed = (oldKeys ^ keys) != 0;
            oldKeys = keys;
            if (!changed) {
                if (timer.getElapsedTime().asSeconds() > 5) {
                    getInput()->setRepeatDelay(INPUT_DELAY / 20);
                } else if (timer.getElapsedTime().asSeconds() > 3) {
                    getInput()->setRepeatDelay(INPUT_DELAY / 8);
                } else if (timer.getElapsedTime().asSeconds() > 1) {
                    getInput()->setRepeatDelay(INPUT_DELAY / 4);
                }
            } else {
                getInput()->setRepeatDelay(INPUT_DELAY);
                timer.restart();
            }
        }
    }

    C2DRenderer::onUpdate();
}

float UiMain::getScaling() {
    return scaling;
}

Skin *UiMain::getSkin() {
    return skin;
}

void UiMain::setConfig(Config *cfg) {
    config = cfg;
}

Config *UiMain::getConfig() {
    return config;
}

UIHighlight *UiMain::getUiHighlight() {
    return uiHighlight;
}

UIRomList *UiMain::getUiRomList() {
    return uiRomList;
}

UIEmu *UiMain::getUiEmu() {
    return uiEmu;
}

UiMenu *UiMain::getUiMenu() {
    return uiMenu;
}

UiStateMenu *UiMain::getUiStateMenu() {
    return uiState;
}

UIProgressBox *UiMain::getUiProgressBox() {
    return uiProgressBox;
}

c2d::MessageBox *UiMain::getUiMessageBox() {
    return uiMessageBox;
}

int UiMain::getFontSize() {
    return (int) ((float) C2D_DEFAULT_CHAR_SIZE * scaling);
}

void UiMain::updateInputMapping(bool isRomConfig) {

    if (isRomConfig) {
#ifndef NO_KEYBOARD
        getInput()->setKeyboardMapping(config->getPlayerInputKeys(0, true));
#endif
        int dz = config->get(Option::Id::JOY_DEADZONE, true)->getValueInt();
        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, config->getPlayerInputButtons(i, true), dz);
            getInput()->players[i].lx.id = config->get(Option::Id::JOY_AXIS_LX, true)->getValueInt();
            getInput()->players[i].ly.id = config->get(Option::Id::JOY_AXIS_LY, true)->getValueInt();
            getInput()->players[i].rx.id = config->get(Option::Id::JOY_AXIS_RX, true)->getValueInt();
            getInput()->players[i].ry.id = config->get(Option::Id::JOY_AXIS_RY, true)->getValueInt();
        }
    } else {
#ifndef NO_KEYBOARD
        // keep custom config for menus keys
        int key_mapping[15];
        memcpy(key_mapping, C2D_DEFAULT_KB_KEYS, sizeof(key_mapping));
        key_mapping[12] = config->get(Option::Id::KEY_MENU1, false)->getValueInt();
        key_mapping[13] = config->get(Option::Id::KEY_MENU2, false)->getValueInt();
        getInput()->setKeyboardMapping(key_mapping);
#endif
        int joy_mapping[15];
        memcpy(joy_mapping, C2D_DEFAULT_JOY_KEYS, sizeof(joy_mapping));
        joy_mapping[12] = config->get(Option::Id::JOY_MENU1, false)->getValueInt();
        joy_mapping[13] = config->get(Option::Id::JOY_MENU2, false)->getValueInt();

        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, joy_mapping);
            //
            getInput()->players[i].lx.id = KEY_JOY_AXIS_LX;
            getInput()->players[i].ly.id = KEY_JOY_AXIS_LY;
            getInput()->players[i].rx.id = KEY_JOY_AXIS_RX;
            getInput()->players[i].ry.id = KEY_JOY_AXIS_RY;
        }
    }
#ifdef __SWITCH__
    bool single_joy_mode = config->get(Option::Id::JOY_SINGLEJOYCON)->getValueBool();
    ((SWITCHInput *) getInput())->setSingleJoyconMode(single_joy_mode);
#endif
}


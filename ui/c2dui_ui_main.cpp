//
// Created by cpasjuste on 22/11/16.
//

#include <algorithm>

#ifdef __SWITCH__
extern "C" {
#include <switch/services/hid.h>
}
#endif

#include "c2dui.h"
#include "c2dui_ui_main.h"


using namespace c2d;
using namespace c2dui;

UIMain::UIMain(const Vector2f &size) : C2DRenderer(size) {

}

UIMain::~UIMain() {
    // ui elements (C2DObject)
    // are deleted by the renderer
#if 0
    delete (scrapper);
#endif
}

void UIMain::init(UIRomList *_uiRomList, UIMenu *_uiMenu,
                  UIEmu *_uiEmu, UIStateMenu *_uiState) {
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
    scaling = std::min(getSize().x / C2D_SCREEN_WIDTH, 1.0f);
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
    uiMessageBox->setSelectedColor(uiMessageBox->getOutlineColor(), COL_RED);
    uiMessageBox->setNotSelectedColor(uiMessageBox->getFillColor(), uiMessageBox->getOutlineColor());
    uiMessageBox->setOrigin(Origin::Center);
    add(uiMessageBox);

    uiProgressBox = new UIProgressBox(this);
    add(uiProgressBox);

    updateInputMapping(false);
    getInput()->setRepeatDelay(INPUT_DELAY);

#if 0
    scrapper = new Scrapper(this);
#endif
}

void UIMain::setSkin(Skin *s) {
    skin = s;
}

void UIMain::onUpdate() {

    if (uiEmu && !uiEmu->isVisible()) {
        unsigned int keys = getInput()->getKeys(0);
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

    Renderer::onUpdate();
}

float UIMain::getScaling() {
    return scaling;
}

Skin *UIMain::getSkin() {
    return skin;
}

void UIMain::setConfig(Config *cfg) {
    config = cfg;
}

Config *UIMain::getConfig() {
    return config;
}

UIHighlight *UIMain::getUiHighlight() {
    return uiHighlight;
}

UIRomList *UIMain::getUiRomList() {
    return uiRomList;
}

UIEmu *UIMain::getUiEmu() {
    return uiEmu;
}

UIMenu *UIMain::getUiMenu() {
    return uiMenu;
}

UIStateMenu *UIMain::getUiStateMenu() {
    return uiState;
}

UIProgressBox *UIMain::getUiProgressBox() {
    return uiProgressBox;
}

c2d::MessageBox *UIMain::getUiMessageBox() {
    return uiMessageBox;
}

int UIMain::getFontSize() {
    return C2D_DEFAULT_CHAR_SIZE;
}

void UIMain::updateInputMapping(bool isRomConfig) {

    if (isRomConfig) {
        getInput()->setKeyboardMapping(config->getPlayerInputKeys(0, true));
        int dz = config->get(Option::Id::JOY_DEADZONE, true)->getValueInt();
        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, config->getPlayerInputButtons(i, true), dz);
            getInput()->players[i].lx.id = config->get(Option::Id::JOY_AXIS_LX, true)->getValueInt();
            getInput()->players[i].ly.id = config->get(Option::Id::JOY_AXIS_LY, true)->getValueInt();
            getInput()->players[i].rx.id = config->get(Option::Id::JOY_AXIS_RX, true)->getValueInt();
            getInput()->players[i].ry.id = config->get(Option::Id::JOY_AXIS_RY, true)->getValueInt();
        }
    } else {
        getInput()->setKeyboardMapping(C2D_DEFAULT_KB_KEYS);
        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, C2D_DEFAULT_JOY_KEYS);
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


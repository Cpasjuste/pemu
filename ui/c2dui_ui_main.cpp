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
                  UiEmu *_uiEmu, UiStateMenu *_uiState) {
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
    float fontSize = uiMessageBox->getTitleText()->getSize().y * 1.5f;
    uiMessageBox->getTitleText()->setSize(
            uiMessageBox->getTitleText()->getSize().x * 1.5f, fontSize);
    uiMessageBox->getTitleText()->setSizeMax(uiMessageBox->getSize().x - (float) fontSize * 2, (float) fontSize + 4);
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

    uiStatusBox = new UiStatusBox(this);
    add(uiStatusBox);

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
    unsigned int buttons = getInput()->getButtons();
    if (buttons & Input::Button::Quit) {
        done = true;
        return C2DRenderer::onUpdate();
    }

    if (uiEmu && (!uiEmu->isVisible() || uiEmu->isPaused())) {
        if (buttons != Input::Button::Delay) {
            bool changed = (oldKeys ^ buttons) != 0;
            oldKeys = buttons;
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

UiEmu *UiMain::getUiEmu() {
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

UiStatusBox *UiMain::getUiStatusBox() {
    return uiStatusBox;
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
        getInput()->setKeyboardMapping(config->getKeyboardMapping(0, true));
#endif
        int dz = config->getJoystickDeadZone(0, true);
        std::vector<Input::ButtonMapping> joyMapping = config->getJoystickMapping(0, true);
        Vector2i leftAxisMapping = config->getJoystickAxisLeftMapping(0, true);
        Vector2i rightAxisMapping = config->getJoystickAxisRightMapping(0, true);
        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, joyMapping, leftAxisMapping, rightAxisMapping, dz);
        }
    } else {
#ifndef NO_KEYBOARD
        std::vector<Input::ButtonMapping> keyMapping = getInput()->getKeyboardMappingDefault();
        // keep custom config for menus keys
        for (unsigned int i = 0; i < keyMapping.size(); i++) {
            if (keyMapping.at(i).button == Input::Button::Menu1) {
                keyMapping.at(i).value = config->get(Option::Id::KEY_MENU1, false)->getValueInt();
            } else if (keyMapping.at(i).button == Input::Button::Menu2) {
                keyMapping.at(i).value = config->get(Option::Id::KEY_MENU2, false)->getValueInt();
            }
        }
        getInput()->setKeyboardMapping(keyMapping);
#endif
        std::vector<Input::ButtonMapping> joyMapping = getInput()->getPlayer(0)->mapping_default;
        // keep custom config for menus keys
        for (unsigned int i = 0; i < joyMapping.size(); i++) {
            if (joyMapping.at(i).button == Input::Button::Menu1) {
                joyMapping.at(i).value = config->get(Option::Id::JOY_MENU1, false)->getValueInt();
            } else if (joyMapping.at(i).button == Input::Button::Menu2) {
                joyMapping.at(i).value = config->get(Option::Id::JOY_MENU2, false)->getValueInt();
            }
        }
        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, joyMapping,
                                           {KEY_JOY_AXIS_LX, KEY_JOY_AXIS_LY},
                                           {KEY_JOY_AXIS_RX, KEY_JOY_AXIS_RY},
                                           config->getJoystickDeadZone(0, false));
        }
    }
}

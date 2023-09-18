//
// Created by cpasjuste on 22/11/16.
//

#include <algorithm>
#include "c2dui.h"
#include "c2dui_ui_main.h"

#ifdef __SWITCH__
UiMain::UiMain(c2d::Io *io, const c2d::Vector2f &size) : C2DRenderer({1280, 720}) {
#else

UiMain::UiMain(c2d::Io *io, const c2d::Vector2f &size) : C2DRenderer(size) {
#endif
    printf("UiMain(%ix%i)\n", (int) UiMain::getSize().x, (int) UiMain::getSize().y);
    setIo(io);
}

void UiMain::init(UIRomList *romList, UiMenu *menu, UiEmu *emu, UiStateMenu *state) {
    pRomList = romList;
    pRomList->updateRomList();
    add(pRomList);

    // build menus from options
    pMenu = menu;
    add(pMenu);

    pEmu = emu;
    add(pEmu);

    pState = state;
    add(pState);

    // message box
    Skin::TextGroup textGroup = pSkin->getText(
            {"MAIN", "ROM_LIST", "TEXT"}); // use rom list text size for message box font size
    Skin::RectangleShapeGroup shape = pSkin->getRectangleShape({"SKIN_CONFIG", "MESSAGEBOX"});
    pMessageBox = new c2d::MessageBox(shape.rect, getInput(), pSkin->getFont(), (int) textGroup.size);
    pSkin->loadRectangleShape(pMessageBox, {"SKIN_CONFIG", "MESSAGEBOX"});
    pMessageBox->setSelectedColor(pMessageBox->getFillColor(), pMessageBox->getOutlineColor());
    Color c = pMessageBox->getOutlineColor();
    c.a -= 150;
    pMessageBox->setNotSelectedColor(pMessageBox->getFillColor(), c);
    pMessageBox->setOrigin(Origin::Center);
    if (getSize().x <= 400) {
        pMessageBox->getTitleText()->setOutlineThickness(1);
        pMessageBox->getMessageText()->setOutlineThickness(1);
    }
    add(pMessageBox);

    pProgressBox = new UIProgressBox(this);
    add(pProgressBox);

    pStatusBox = new UiStatusBox(this);
    add(pStatusBox);

    updateInputMapping(false);
    getInput()->setRepeatDelay(INPUT_DELAY);
}

void UiMain::setSkin(Skin *s) {
    pSkin = s;
}

void UiMain::onUpdate() {
    unsigned int buttons = getInput()->getButtons();
    if (buttons & Input::Button::Quit) {
        done = true;
        return C2DRenderer::onUpdate();
    }

    if (pEmu && (!pEmu->isVisible() || pEmu->isPaused())) {
        if (buttons != Input::Button::Delay) {
            bool changed = (mOldKeys ^ buttons) != 0;
            mOldKeys = buttons;
            if (!changed) {
                if (mTimer.getElapsedTime().asSeconds() > 5) {
                    getInput()->setRepeatDelay(INPUT_DELAY / 20);
                } else if (mTimer.getElapsedTime().asSeconds() > 3) {
                    getInput()->setRepeatDelay(INPUT_DELAY / 8);
                } else if (mTimer.getElapsedTime().asSeconds() > 1) {
                    getInput()->setRepeatDelay(INPUT_DELAY / 4);
                }
            } else {
                getInput()->setRepeatDelay(INPUT_DELAY);
                mTimer.restart();
            }
        }
    }

    C2DRenderer::onUpdate();
}

Skin *UiMain::getSkin() {
    return pSkin;
}

void UiMain::setConfig(PEMUConfig *cfg) {
    pConfig = cfg;

    // add shaders, if any
    auto shaderList = getShaderList();
    if (shaderList) {
        pConfig->getGroup(PEMUConfig::Id::MENU_ROM_OPTIONS)->addOption(
                {"EFFECT", shaderList->getNames(), 0, PEMUConfig::Id::ROM_SHADER});
    } else {
        pConfig->getGroup(PEMUConfig::Id::MENU_ROM_OPTIONS)->addOption(
                {"EFFECT", {"NONE"}, 0, PEMUConfig::Id::ROM_SHADER})->setFlags(PEMUConfig::Flags::HIDDEN);
    }
}

PEMUConfig *UiMain::getConfig() {
    return pConfig;
}

UIHighlight *UiMain::getUiHighlight() {
    return pHighlight;
}

UIRomList *UiMain::getUiRomList() {
    return pRomList;
}

UiEmu *UiMain::getUiEmu() {
    return pEmu;
}

UiMenu *UiMain::getUiMenu() {
    return pMenu;
}

UiStateMenu *UiMain::getUiStateMenu() {
    return pState;
}

UIProgressBox *UiMain::getUiProgressBox() {
    return pProgressBox;
}

UiStatusBox *UiMain::getUiStatusBox() {
    return pStatusBox;
}

c2d::MessageBox *UiMain::getUiMessageBox() {
    return pMessageBox;
}

int UiMain::getFontSize() {
    return (int) ((float) C2D_DEFAULT_CHAR_SIZE * pSkin->getScaling().y);
}

Vector2f UiMain::getScaling() {
    return pSkin->getScaling();
}

void UiMain::updateInputMapping(bool isRomConfig) {
    if (isRomConfig) {
#ifndef NO_KEYBOARD
        getInput()->setKeyboardMapping(pConfig->getKeyboardMapping(0, true));
#endif
        int dz = pConfig->getJoystickDeadZone(0, true);
        std::vector<Input::ButtonMapping> joyMapping = pConfig->getJoystickMapping(0, true);
        Vector2i leftAxisMapping = pConfig->getJoystickAxisLeftMapping(0, true);
        Vector2i rightAxisMapping = pConfig->getJoystickAxisRightMapping(0, true);
        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, joyMapping, leftAxisMapping, rightAxisMapping, dz);
        }
    } else {
#ifndef NO_KEYBOARD
        std::vector<Input::ButtonMapping> keyMapping = getInput()->getKeyboardMappingDefault();
        // keep custom config for menus keys
        for (unsigned int i = 0; i < keyMapping.size(); i++) {
            if (keyMapping.at(i).button == Input::Button::Menu1) {
                keyMapping.at(i).value = pConfig->get(PEMUConfig::Id::KEY_MENU1, false)->getInteger();
            } else if (keyMapping.at(i).button == Input::Button::Menu2) {
                keyMapping.at(i).value = pConfig->get(PEMUConfig::Id::KEY_MENU2, false)->getInteger();
            }
        }
        getInput()->setKeyboardMapping(keyMapping);
#endif
        std::vector<Input::ButtonMapping> joyMapping = getInput()->getPlayer(0)->mapping_default;
        // keep custom config for menus keys
        for (unsigned int i = 0; i < joyMapping.size(); i++) {
            if (joyMapping.at(i).button == Input::Button::Menu1) {
                joyMapping.at(i).value = pConfig->get(PEMUConfig::Id::JOY_MENU1, false)->getInteger();
            } else if (joyMapping.at(i).button == Input::Button::Menu2) {
                joyMapping.at(i).value = pConfig->get(PEMUConfig::Id::JOY_MENU2, false)->getInteger();
            }
        }
        for (int i = 0; i < PLAYER_MAX; i++) {
            getInput()->setJoystickMapping(i, joyMapping,
                                           {KEY_JOY_AXIS_LX, KEY_JOY_AXIS_LY},
                                           {KEY_JOY_AXIS_RX, KEY_JOY_AXIS_RY},
                                           pConfig->getJoystickDeadZone(0, false));
        }
    }
}

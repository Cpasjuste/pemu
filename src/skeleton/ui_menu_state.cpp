//
// Created by cpasjuste on 16/02/18.
//

#include "pemu.h"

#define STATES_COUNT 4

class UiState : public SkinnedRectangle {
public:
    UiState(UiMain *ui, float x, int id) : SkinnedRectangle(ui, {"STATES_MENU", "STATES_ITEM"}) {
        this->ui = ui;
        this->id = id;

        UiState::setPosition(x, UiState::getPosition().y);
        outlineColor = UiState::getOutlineColor();
        outlineTickness = UiState::getOutlineThickness();
        outlineColorSelected = ui->getSkin()->getRectangleShape({"STATES_MENU"}).outlineColor;

        // text if no save/screenshot
        middle_text = new C2DText("NO SAVE", 26, ui->getSkin()->getFont());
        ui->getSkin()->loadText(middle_text, {"STATES_MENU", "STATES_ITEM", "STATES_TEXT"});
        middle_text->setOrigin(Origin::Center);
        middle_text->setPosition(UiState::getLocalBounds().left + UiState::getSize().x / 2,
                                 UiState::getLocalBounds().top + UiState::getSize().y / 2);
        UiState::add(middle_text);

        // text for slot id
        char bottom_text_char[32];
        snprintf(bottom_text_char, 32, "SLOT %i", id);
        bottom_text = new C2DText(bottom_text_char, 26, ui->getSkin()->getFont());
        ui->getSkin()->loadText(bottom_text, {"STATES_MENU", "STATES_ITEM", "STATES_TEXT"});
        bottom_text->setString(bottom_text_char);
        bottom_text->setOrigin(Origin::Center);
        bottom_text->setPosition(UiState::getLocalBounds().left + UiState::getSize().x / 2,
                                 UiState::getLocalBounds().top + UiState::getLocalBounds().height + 16);
        UiState::add(bottom_text);
    }

    void loadTexture() {
        if (texture) {
            delete (texture);
            texture = nullptr;
        }

        exist = ui->getIo()->exist(path);
        if (exist) {
            if (ui->getIo()->exist(shot)) {
                texture = new C2DTexture(shot);
                if (texture->available) {
                    float tex_scaling = std::min(
                            getSize().x / (float) texture->getTextureRect().width,
                            getSize().y / (float) texture->getTextureRect().height);
                    texture->setScale(tex_scaling, tex_scaling);
                    texture->setPosition(getSize().x / 2, getSize().y / 2);
                    texture->setOrigin(Origin::Center);
                    add(texture);
                    middle_text->setVisibility(Visibility::Hidden);
                }
            }
            if (!texture || !texture->available) {
                middle_text->setString("NO PIC");
                middle_text->setOrigin(Origin::Center);
                middle_text->setVisibility(Visibility::Visible);
            }
        } else {
            middle_text->setString("NO SAVE");
            middle_text->setOrigin(Origin::Center);
            middle_text->setVisibility(Visibility::Visible);
        }
    }

    void setRom(const ss_api::Game &game) {
        memset(path, 0, MAX_PATH);
        memset(shot, 0, MAX_PATH);
        snprintf(path, 1023, "%ssaves/%s-%i.sav",
                 ui->getIo()->getDataPath().c_str(), Utility::removeExt(game.path).c_str(), id);
        snprintf(shot, 1023, "%ssaves/%s-%i.png",
                 ui->getIo()->getDataPath().c_str(), Utility::removeExt(game.path).c_str(), id);

        loadTexture();
    }

    void loadState() {
        printf("StateLoad: %s\n", path);
        ui->getUiStateMenu()->loadStateCore(path);
    }

    void saveState() {
        printf("StateSave: %s\n", path);
        if (ui->getUiStateMenu()->saveStateCore(path)) {
            ui->getUiEmu()->getVideo()->save(shot);
            loadTexture();
        }
    }

    UiMain *ui;
    Texture *texture = nullptr;
    Text *middle_text = nullptr;
    Text *bottom_text = nullptr;
    float outlineTickness;
    Color outlineColor;
    Color outlineColorSelected;

    char path[1024]{};
    char shot[1024]{};
    bool exist = false;
    int id = 0;
};

class UiStateList : public RectangleShape {
public:
    UiStateList(UiMain *ui, const FloatRect &rect) : RectangleShape(rect) {
        UiStateList::setFillColor(Color::Transparent);

        // add states items
        float width = UiStateList::getSize().x / STATES_COUNT;
        for (int i = 0; i < STATES_COUNT; i++) {
            states[i] = new UiState(ui, (width * (float) i) + (width / 2), i);
            states[i]->setOrigin(Origin::Center);
            UiStateList::add(states[i]);
        }

        setSelection(0);
    }

    ~UiStateList() override {
        for (auto &state: states) {
            delete state;
        }
    }

    UiState *getSelection() {
        return states[index];
    }

    void setSelection(int idx) {
        if (idx < 0 || idx > STATES_COUNT) {
            return;
        }

        index = idx;
        for (int i = 0; i < STATES_COUNT; i++) {
            states[i]->setOutlineColor(i == index ? states[i]->outlineColorSelected : states[i]->outlineColor);
            states[i]->setOutlineThickness(i == index ? states[i]->outlineTickness * 2 : states[i]->outlineTickness);
            states[i]->setLayer(i == index ? 1 : 0);
            float scale = i == index ? 1.0f : 0.95f;
            states[i]->setScale(scale, scale);
        }
    }

    void left() {
        index--;
        if (index < 0) {
            index = STATES_COUNT - 1;
        }
        setSelection(index);
    }

    void right() {
        index++;
        if (index > STATES_COUNT - 1) {
            index = 0;
        }
        setSelection(index);
    }

    UiState *states[STATES_COUNT]{};
    int index = 0;
};

UiMenuState::UiMenuState(UiMain *u) : SkinnedRectangle(u, {"STATES_MENU"}) {
    printf("UIStateMenu()\n");

    ui = u;

    // menu title
    title = new SkinnedText(ui, {"STATES_MENU", "TITLE_TEXT"});
    title->setString("TITLE");
    title->setStyle(Text::Underlined);
    UiMenuState::add(title);

    float start_y = title->getLocalBounds().top + title->getLocalBounds().height + (32 * ui->getScaling().y);
    uiStateList = new UiStateList(ui, {
            UiMenuState::getLocalBounds().left + UiMenuState::getSize().x / 2, start_y,
            UiMenuState::getSize().x - (64 * ui->getScaling().x), UiMenuState::getSize().x / (STATES_COUNT + 1)
    });
    uiStateList->setOrigin(Origin::Top);
    UiMenuState::add(uiStateList);

    auto tweenPosition = new TweenPosition(UiMenuState::getPosition(),
                                           {UiMenuState::getPosition().x, ui->getSize().y - UiMenuState::getSize().y},
                                           0.2f);
    tweenPosition->setState(TweenState::Stopped);
    UiMenuState::add(tweenPosition);

    UiMenuState::setVisibility(Visibility::Hidden);
}

void UiMenuState::setVisibility(c2d::Visibility visibility, bool tweenPlay) {
    if (ui->getUiRomList() && ui->getUiRomList()->isVisible()) {
        ui->getUiRomList()->getBlur()->setVisibility(visibility, true);
    }
    if (ui->getUiEmu() && visibility == Visibility::Visible) {
        title->setString(ui->getUiRomList()->getSelection().name);
        for (auto &state: uiStateList->states) {
            state->setRom(ui->getUiRomList()->getSelection());
        }
        uiStateList->setSelection(0);
    }

    RectangleShape::setVisibility(visibility, tweenPlay);
}

bool UiMenuState::onInput(c2d::Input::Player *players) {
    unsigned int buttons = players[0].buttons;

    if (buttons & Input::Button::Left) {
        uiStateList->left();
    } else if (buttons & Input::Button::Right) {
        uiStateList->right();
    }

    // FIRE1
    if (buttons & Input::Button::A) {
        isEmuRunning = ui->getUiEmu()->isVisible();
        if (isEmuRunning) {
            UiState *state = uiStateList->getSelection();
            if (state->exist) {
                int res = ui->getUiMessageBox()->show(
                        state->bottom_text->getString(),
                        "PRESS FIRE2 TO CANCEL", "LOAD", "SAVE");
                if (res == MessageBox::LEFT) {
                    state->loadState();
                    setVisibility(Visibility::Hidden);
                    ui->getUiEmu()->resume();
                } else if (res == MessageBox::RIGHT) {
                    state->saveState();
                    setVisibility(Visibility::Hidden);
                    ui->getUiEmu()->resume();
                }
            } else {
                state->saveState();
                setVisibility(Visibility::Hidden);
                ui->getUiEmu()->resume();
            }
        } else {
            ss_api::Game game = ui->getUiRomList()->getSelection();
            if (game.available) {
                UiState *state = uiStateList->getSelection();
                if (state->exist) {
                    ui->getConfig()->loadGame(game);
                    ui->getUiEmu()->load(game);
                    state->loadState();
                    setVisibility(Visibility::Hidden);
                    return true;
                }
            }
        }
    }

    // FIRE2
    if (buttons & Input::Button::B) {
        setVisibility(Visibility::Hidden, true);
        ui->getUiMenu()->setVisibility(Visibility::Visible, true);
    }

    return true;
}

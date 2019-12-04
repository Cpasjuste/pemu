//
// Created by cpasjuste on 16/02/18.
//

#include "c2dui.h"

#define STATES_COUNT 4

using namespace c2d;
using namespace c2dui;

class UIState : public RectangleShape {

public:

    UIState(UIMain *ui, const FloatRect &rect, int id) : RectangleShape(rect) {

        this->ui = ui;
        this->id = id;

        ui->getSkin()->loadRectangleShape(this, {"STATES_MENU", "STATES_ITEM"});
        setSize(rect.width, rect.height);
        setPosition(rect.left, rect.top);
        outlineColor = getOutlineColor();
        outlineTickness = getOutlineThickness();
        outlineColorSelected = ui->getSkin()->getRectangleShape({"STATES_MENU"}).outlineColor;

        // text if no save/screenshot
        middle_text = new C2DText("NO SAVE", 26, ui->getSkin()->getFont());
        ui->getSkin()->loadText(middle_text, {"STATES_MENU", "STATES_ITEM", "STATES_TEXT"});
        middle_text->setOrigin(Origin::Center);
        middle_text->setPosition(getLocalBounds().left + getSize().x / 2,
                                 getLocalBounds().top + getSize().y / 2);
        add(middle_text);

        // text for slot id
        char bottom_text_char[32];
        snprintf(bottom_text_char, 32, "SLOT %i", id);
        bottom_text = new C2DText(bottom_text_char, 26, ui->getSkin()->getFont());
        ui->getSkin()->loadText(bottom_text, {"STATES_MENU", "STATES_ITEM", "STATES_TEXT"});
        bottom_text->setString(bottom_text_char);
        bottom_text->setOrigin(Origin::Center);
        bottom_text->setPosition(getLocalBounds().left + getSize().x / 2,
                                 getLocalBounds().top + getLocalBounds().height + 16);
        add(bottom_text);
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
                            getSize().x / texture->getTextureRect().width,
                            getSize().y / texture->getTextureRect().height);
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
        snprintf(path, 1023, "%ssaves/%s%i.sav",
                 ui->getConfig()->getHomePath().c_str(), Utility::removeExt(game.path).c_str(), id);
        snprintf(shot, 1023, "%ssaves/%s%i.png",
                 ui->getConfig()->getHomePath().c_str(), Utility::removeExt(game.path).c_str(), id);

        loadTexture();
    }

    void load() {
        printf("StateLoad: %s\n", path);
        ui->getUiStateMenu()->loadStateCore(path);
    }

    void save() {
        printf("StateSave: %s\n", path);
        if (ui->getUiStateMenu()->saveStateCore(path)) {
            ui->getUiEmu()->getVideo()->save(shot);
            loadTexture();
        }
    }

    UIMain *ui;
    Texture *texture = nullptr;
    Text *middle_text = nullptr;
    Text *bottom_text = nullptr;
    float outlineTickness;
    Color outlineColor;
    Color outlineColorSelected;

    char path[1024];
    char shot[1024];
    bool exist = false;
    int id = 0;
};

class UIStateList : public RectangleShape {

public:

    UIStateList(UIMain *ui, const FloatRect &rect) : RectangleShape(rect) {

        setFillColor(Color::Transparent);

        // add states items
        float width = getSize().x / STATES_COUNT;
        for (int i = 0; i < STATES_COUNT; i++) {
            FloatRect r = {(width * i) + (width / 2), width / 2, width, width};
            states[i] = new UIState(ui, r, i);
            states[i]->setOrigin(Origin::Center);
            add(states[i]);
        }

        setSelection(0);
    }

    ~UIStateList() {
        for (auto &state : states) {
            delete state;
        }
    }

    UIState *getSelection() {
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

    UIState *states[STATES_COUNT];
    int index = 0;
};

UIStateMenu::UIStateMenu(UIMain *u) : RectangleShape(Vector2f(16, 16)) {

    printf("UIStateMenu()\n");

    this->ui = u;
    Skin *skin = ui->getSkin();

    // main rect/bg
    skin->loadRectangleShape(this, {"STATES_MENU"});

    // menu title
    title = new Text("TITLE", C2D_DEFAULT_CHAR_SIZE, ui->getSkin()->font);
    title->setStyle(Text::Underlined);
    skin->loadText(title, {"STATES_MENU", "TITLE_TEXT"});
    add(title);

    int start_y = (int) (title->getGlobalBounds().top + title->getGlobalBounds().height + 16 * ui->getScaling());

    uiStateList = new UIStateList(ui, {
            getLocalBounds().left + getSize().x / 2, (float) start_y + 32,
            getSize().x - 64, getSize().x / (STATES_COUNT + 1)
    });
    uiStateList->setOrigin(Origin::Top);
    add(uiStateList);

    auto tweenPosition = new TweenPosition({getPosition().x, getSize().y * 2}, getPosition(), 0.2f);
    tweenPosition->setState(TweenState::Stopped);
    add(tweenPosition);

    setVisibility(Visibility::Hidden);
}

void UIStateMenu::setVisibility(c2d::Visibility visibility, bool tweenPlay) {

    if (ui->getUiEmu()) {
        if (visibility == Visibility::Visible) {
            title->setString(ui->getUiRomList()->getSelection().getName().text);
            for (auto &state : uiStateList->states) {
                state->setRom(ui->getUiRomList()->getSelection());
            }
            uiStateList->setSelection(0);
        }
    }

    RectangleShape::setVisibility(visibility, tweenPlay);
}

bool UIStateMenu::onInput(c2d::Input::Player *players) {

    unsigned int key = players[0].keys;

    if (key & Input::Key::Left) {
        uiStateList->left();
    } else if (key & Input::Key::Right) {
        uiStateList->right();
    }

    // FIRE1
    if (key & Input::Key::Fire1) {
        isEmuRunning = ui->getUiEmu()->isVisible();
        if (isEmuRunning) {
            UIState *state = uiStateList->getSelection();
            if (state->exist) {
                int res = ui->getUiMessageBox()->show(
                        state->bottom_text->getString(),
                        "PRESS FIRE2 TO CANCEL", "LOAD", "SAVE");
                if (res == MessageBox::LEFT) {
                    state->load();
                    setVisibility(Visibility::Hidden);
                    ui->getUiEmu()->resume();
                } else if (res == MessageBox::RIGHT) {
                    state->save();
                    setVisibility(Visibility::Hidden);
                    ui->getUiEmu()->resume();
                }
            } else {
                state->save();
                setVisibility(Visibility::Hidden);
                ui->getUiEmu()->resume();
            }
        } else {
            ss_api::Game game = ui->getUiRomList()->getSelection();
            if (game.available) {
                UIState *state = uiStateList->getSelection();
                if (state->exist) {
                    ui->getConfig()->load(game);
                    ui->getUiEmu()->load(game);
                    state->load();
                    setVisibility(Visibility::Hidden);
                    return true;
                }
            }
        }
    }

    // FIRE2
    if (key & Input::Key::Fire2) {
        setVisibility(Visibility::Hidden, true);
        ui->getUiMenu()->setVisibility(Visibility::Visible, true);
    }

    // QUIT
    if (key & EV_QUIT) {
        return EV_QUIT;
    }

    return true;
}

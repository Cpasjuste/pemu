//
// Created by cpasjuste on 22/11/16.
//

#include "c2dui.h"

#define ROM_ITEM_MARGIN 32

/// pFBA
#define BDF_ORIENTATION_FLIPPED     (1 << 1)
#define BDF_ORIENTATION_VERTICAL    (1 << 2)

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

class UIRomItem : public RoundedRectangleShape {

public:

    UIRomItem(UIMain *u, const Game &g) : RoundedRectangleShape() {
        ui = u;
        game = g;
    }

    void setPreview(Texture *tex) {

        if (preview) {
            delete (preview);
            preview = nullptr;
        }

        if (!tex) {
            return;
        }

        preview = tex;
        if (preview->available) {
            preview->setOrigin(Origin::Top);
            preview->setPosition(getSize().x / 2, 0);
            float scale = std::min(
                    getSize().x / preview->getSize().x,
                    ((getSize().y / 5) * 4) / preview->getSize().y);
            preview->setScale(scale, scale);
            add(preview);
        } else {
            delete (preview);
            preview = nullptr;
        }
    }

private:

    void onDraw(Transform &transform, bool draw) override {

        // draw from -1 item to screen size + 1 item
        FloatRect bounds = getGlobalBounds();
        if (bounds.left > -(getSize().x + ROM_ITEM_MARGIN) &&
            bounds.left < ui->getSize().x + getSize().x + ROM_ITEM_MARGIN) {
            if (!previewLoaded && ui->getInput()->getRepeatDelay() >= INPUT_DELAY / 2) {
                setPreview(ui->getUiRomList()->getPreviewTexture(game));
                previewLoaded = true;
            }
            Shape::onDraw(transform, draw);
        } else {
            if (previewLoaded) {
                setPreview(nullptr);
                previewLoaded = false;
            }
            Shape::onDraw(transform, false);
        }
    }

    UIMain *ui = nullptr;
    Game game;
    Texture *preview = nullptr;
    bool previewLoaded = false;
};


UIRomListNew::UIRomListNew(UIMain *u, RomList *romList, const c2d::Vector2f &size)
        : UIRomList(u, romList, size) {

    printf("UIRomListNew\n");

    ui = u;
    rom_list = romList;

    // load bg skin
    ui->getSkin()->loadRectangleShape(this, {"MAIN"});

    auto titleRect = new RoundedRectangleShape();
    ui->getSkin()->loadRectangleShape(titleRect, {"MAIN", "TITLE"});
    title = new Text("NO ROM INFORMATION", (unsigned int) ui->getFontSize(), ui->getSkin()->getFont());
    ui->getSkin()->loadText(title, {"MAIN", "TITLE", "TEXT"});
    titleRect->add(title);
    add(titleRect);
}

void UIRomListNew::updateRomList() {

    /*
    filterRomList();

    rom_items.clear();
    delete (rom_items_layer);
    rom_items_layer_y = ui->getSkin()->getConfig()->getGroup("MAIN")->getOption("ROM_LIST", "position_y")->getFloat();
    rom_items_layer = new Rectangle({0, rom_items_layer_y, 16, 16});
    rom_items_layer->setOrigin(Origin::Left);
    rom_items_layer_tween = new TweenPosition(rom_items_layer->getPosition(), rom_items_layer->getPosition(), 0.25f);
    rom_items_layer_tween->setState(TweenState::Stopped);
    rom_items_layer->add(rom_items_layer_tween);
    add(rom_items_layer);

    float start_x = ROM_ITEM_MARGIN;
    for (unsigned int i = 0; i < roms.size(); i++) {
        auto *item = new UIRomItem(ui, roms[i]);
        // TODO: not in config anymore
        ui->getSkin()->loadRectangleShape(item, {"MAIN", "ROM_LIST", "TEXT"});
        rom_item_size = item->getSize();
        rom_item_outline = item->getOutlineThickness();
        item->setOrigin(Origin::Left);
        item->setPosition(start_x + ((rom_item_size.x + ROM_ITEM_MARGIN) * i), rom_items_layer->getSize().y / 2);
        rom_items.push_back(item);
        rom_items_layer->add(item);
    }

    rom_index = 0;
    if (!roms.empty()) {
        ui->getUiHighlight()->tweenPosition(getHighlightPosition(0), 1);
        ui->getUiHighlight()->setVisibility(Visibility::Visible);
        title->setString(getSelection()->name);
    } else {
        ui->getUiHighlight()->setVisibility(Visibility::Hidden);
        title->setString("NO ROM FOUND");
    }
    */
}

Game UIRomListNew::getSelection() {
    return Game();
    //return roms.at((size_t) rom_index);
}

FloatRect UIRomListNew::getHighlightPosition(int index) {

    float width = rom_item_size.x + rom_item_outline * 2;
    float height = rom_item_size.y + rom_item_outline * 2;
    float x = (ROM_ITEM_MARGIN + (index * (rom_item_size.x + ROM_ITEM_MARGIN)))
              + (width / 2) + (ui->getUiHighlight()->getOutlineThickness() / 2);
    float y = rom_items_layer_y + rom_item_outline + getPosition().y;
    return {x, y, width, height};
}

bool UIRomListNew::onInput(c2d::Input::Player *players) {

    /*
    if (ui->getUiMenu()->isVisible()) {
        return false;
    }

    unsigned int keys = players[0].keys;

    if (keys & Input::Left) {
        if (roms.empty()) {
            return true;
        }
        rom_index--;
        if (rom_index < 0) {
            rom_index = (int) (roms.size() - 1);
            int index = roms.size() < 5 ? (int) roms.size() - 1 : 4;
            highlight_index = index;
            if (roms.size() > 4) {
                FloatRect rect = getHighlightPosition(rom_index - index);
                rom_items_layer->setPosition(
                        -rect.left + rom_item_size.x / 2 + ROM_ITEM_MARGIN + rom_item_outline * 2,
                        rom_items_layer_y);
                rom_items_layer_tween->reset();
                rom_items_layer_tween->setState(TweenState::Stopped);
                Vector2f pos = rom_items_layer->getPosition();
                rom_items_layer_tween->setFromTo(pos, pos, ((float) ui->getInput()->getRepeatDelay() * 0.001f) / 3);
            }
            int ms = ui->getInput()->getRepeatDelay() / 3;
            ui->getUiHighlight()->tweenPosition(getHighlightPosition(highlight_index), ms);
        } else if (highlight_index != 0) {
            highlight_index--;
            int ms = ui->getInput()->getRepeatDelay() / 3;
            ui->getUiHighlight()->tweenPosition(getHighlightPosition(highlight_index), ms);
        } else {
            Vector2f pos = rom_items_layer->getPosition();
            Vector2f to = {rom_items_layer_tween->getTo().x + (ROM_ITEM_MARGIN + rom_item_size.x), pos.y};
            rom_items_layer_tween->setFromTo(pos, to, ((float) ui->getInput()->getRepeatDelay() * 0.001f) / 3);
            rom_items_layer_tween->play(TweenDirection::Forward, true);
        }
        title->setString(getSelection()->name);
    } else if (keys & Input::Right) {
        if (roms.empty()) {
            return true;
        }
        rom_index++;
        if (rom_index >= (int) roms.size()) {
            rom_index = 0;
            highlight_index = 0;
            rom_items_layer->setPosition(0, rom_items_layer_y);
            rom_items_layer_tween->reset();
            rom_items_layer_tween->setState(TweenState::Stopped);
            Vector2f pos = rom_items_layer->getPosition();
            rom_items_layer_tween->setFromTo(pos, pos, ((float) ui->getInput()->getRepeatDelay() * 0.001f) / 3);
            int ms = ui->getInput()->getRepeatDelay() / 3;
            ui->getUiHighlight()->tweenPosition(getHighlightPosition(highlight_index), ms);
        } else if (highlight_index != 4) {
            highlight_index++;
            int ms = ui->getInput()->getRepeatDelay() / 3;
            ui->getUiHighlight()->tweenPosition(getHighlightPosition(highlight_index), ms);
        } else {
            Vector2f pos = rom_items_layer->getPosition();
            Vector2f to = {rom_items_layer_tween->getTo().x - (ROM_ITEM_MARGIN + rom_item_size.x), pos.y};
            rom_items_layer_tween->setFromTo(pos, to, ((float) ui->getInput()->getRepeatDelay() * 0.001f) / 3);
            rom_items_layer_tween->play(TweenDirection::Forward, true);
        }
        title->setString(getSelection()->name);
    } else if (keys & Input::Key::Fire1) {
        RomList::Rom *rom = getSelection();
        if (rom && rom->state != RomList::RomState::MISSING) {
            ui->getConfig()->load(rom);
            ui->getUiEmu()->load(rom);
            ui->getUiHighlight()->setVisibility(Visibility::Hidden);
            return true;
        }
    }

    if (keys & EV_QUIT) {
        ui->done = true;
    }
    */
    return true;
}

void UIRomListNew::onDraw(c2d::Transform &transform, bool draw) {
    RectangleShape::onDraw(transform, draw);
}

UIRomListNew::~UIRomListNew() {
    printf("~UIRomListNew\n");
}

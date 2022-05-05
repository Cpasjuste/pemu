//
// Created by cpasjuste on 08/12/18.
//

#include "c2dui.h"

UiHelp::UiHelp(UiMain *ui) : SkinnedRectangle(ui->getSkin(), {"MAIN", "HELP"}) {
    auto font = ui->getSkin()->getFont();

    // navigation
    float pos = addItem(true, font, {ui->getSkin()->getButton(100)},
                        "NAVIGATION", {4, getSize().y / 2});
    // run
    pos = addItem(true, font, {ui->getSkin()->getButton(KEY_JOY_A_DEFAULT)},
                  "RUN", {pos + 12, getSize().y / 2});
    // favorite
    pos = addItem(true, font, {ui->getSkin()->getButton(KEY_JOY_Y_DEFAULT)},
                  "ADD / REMOVE FAVORITE", {pos + 12, getSize().y / 2});
    if (!(ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->getFlags() & Option::Flags::HIDDEN)) {
        // system
        addItem(true, font,
                {ui->getSkin()->getButton(KEY_JOY_LT_DEFAULT), ui->getSkin()->getButton(KEY_JOY_RT_DEFAULT)},
                "SWITCH SYSTEM", {pos + 12, getSize().y / 2});
    }

    // main menu
    pos = addItem(false, font, {ui->getSkin()->getButton(KEY_JOY_START_DEFAULT)},
                  "MAIN MENU", {getSize().x - 4, getSize().y / 2});

    addItem(false, font, {ui->getSkin()->getButton(KEY_JOY_SELECT_DEFAULT)},
            "ROM MENU", {pos - 12, getSize().y / 2});
}

float
UiHelp::addItem(bool left, c2d::Font *font, const std::vector<Skin::Button *> &buttons,
                const std::string &name, const c2d::Vector2f &position) {
    FloatRect bounds;
    Vector2f pos = position;

    for (const auto &button: buttons) {
        if (button && button->texture) {
            auto sprite = new Sprite(button->texture);
            float scaling = std::min((getSize().x - 4) / (float) button->texture->getTextureRect().width,
                                     (getSize().y - 4) / (float) button->texture->getTextureRect().height);
            sprite->setScale(scaling, scaling);
            sprite->setOrigin(left ? Origin::Left : Origin::Right);
            sprite->setPosition(pos);
            add(sprite);
            bounds = sprite->getGlobalBounds();
            pos.x += left ? bounds.width : -bounds.width;
        } else {
            // TODO
            //auto text = new Text("NAVIGATION", (unsigned int) navigationSpriteBounds.height - 10);
        }
    }

    auto text = new Text(name, (unsigned int) bounds.height - 12, font);
    text->setOutlineThickness(1);
    text->setOrigin(left ? Origin::Left : Origin::Right);
    float x = left ? bounds.left + bounds.width + 4 : bounds.left - 4;
    text->setPosition({x, getSize().y / 2});
    add(text);

    return left ? text->getPosition().x + text->getSize().x : text->getPosition().x - text->getSize().x;
}

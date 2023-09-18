//
// Created by cpasjuste on 08/12/18.
//

#include "c2dui.h"

UiHelp::UiHelp(UiMain *ui) : SkinnedRectangle(ui, {"MAIN", "HELP"}) {
    int button1, button2;
    auto font = ui->getSkin()->getFont();

    // navigation
    float pos = addItem(true, font, {ui->getSkin()->getButton(100)},
                        "NAVIGATION", {4, getSize().y / 2});
    // run
    button1 = ui->getConfig()->getOption(PEMUConfig::Id::JOY_A)->getInteger();
    pos = addItem(true, font, {ui->getSkin()->getButton(button1)},
                  "RUN", {pos + 12, getSize().y / 2});
    // favorite
    button1 = ui->getConfig()->getOption(PEMUConfig::Id::JOY_X)->getInteger();
    if (ui->getSize().x > 640) {
        pos = addItem(true, font, {ui->getSkin()->getButton(button1)},
                      "ADD / REMOVE FAVORITE", {pos + 12, getSize().y / 2});
    } else {
        pos = addItem(true, font, {ui->getSkin()->getButton(button1)},
                      "FAVORITE", {pos + 12, getSize().y / 2});
    }

    if (ui->getSize().x > 640) {
        if (!(ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_SYSTEM)->getFlags() & PEMUConfig::Flags::HIDDEN)) {
            // system
            button1 = ui->getConfig()->getOption(PEMUConfig::Id::JOY_LT)->getInteger();
            button2 = ui->getConfig()->getOption(PEMUConfig::Id::JOY_RT)->getInteger();
            addItem(true, font,
                    {ui->getSkin()->getButton(button1), ui->getSkin()->getButton(button2)},
                    "SWITCH SYSTEM", {pos + 12, getSize().y / 2});
        }
    }

    // main menu
    button1 = ui->getConfig()->getOption(PEMUConfig::Id::JOY_MENU1)->getInteger();
    pos = addItem(false, font, {ui->getSkin()->getButton(button1)},
                  "MAIN MENU", {getSize().x - 4, getSize().y / 2});

    button1 = ui->getConfig()->getOption(PEMUConfig::Id::JOY_MENU2)->getInteger();
    addItem(false, font, {ui->getSkin()->getButton(button1)},
            "ROM MENU", {pos - 12, getSize().y / 2});
}

float UiHelp::addItem(bool left, c2d::Font *font, const std::vector<Skin::Button *> &buttons,
                      const std::string &name, const c2d::Vector2f &position) {
    FloatRect bounds;
    Vector2f pos = position;

    for (const auto &button: buttons) {
        if (button && button->texture && button->texture->available) {
            auto sprite = new Sprite(button->texture);
            float scaling = std::min(getSize().x / (float) sprite->getSize().x,
                                     getSize().y / (float) sprite->getSize().y);
            sprite->setScale(scaling, scaling);
            sprite->setOrigin(left ? Origin::Left : Origin::Right);
            sprite->setPosition(pos);
            add(sprite);
            bounds = sprite->getGlobalBounds();
            pos.x += left ? bounds.width : -bounds.width;
        } else {
            // dummy button (TODO)
        }
    }

    auto text = new Text(name, (unsigned int) (getSize().y * 0.85f), font);
    text->setOutlineThickness(1);
    text->setOrigin(left ? Origin::Left : Origin::Right);
    float x = left ? bounds.left + bounds.width : bounds.left;
    text->setPosition({x, getSize().y / 2});
    add(text);

    return left ? text->getPosition().x + text->getSize().x : text->getPosition().x - text->getSize().x;
}

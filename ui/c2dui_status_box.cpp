//
// Created by cpasjuste on 08/12/18.
//

#include "c2dui.h"

UiStatusBox::UiStatusBox(UiMain *m)
        : SkinnedRectangle(m->getSkin(), {"SKIN_CONFIG", "STATUSBOX"}) {

    main = m;
    clock = new C2DClock();

    SkinnedRectangle::setCornersRadius(8);
    SkinnedRectangle::setCornerPointCount(8);

    text = new Text("TIPS:", (int) (getSize().y * 0.65f), main->getFont());
    text->setOutlineThickness(1.0f * main->getScaling());
    text->setPosition(4 * main->getScaling(), (getSize().y / 2) + text->getOutlineThickness());
    text->setOrigin(Origin::Left);
    text->setFillColor(SkinnedRectangle::getOutlineColor());
    add(text);

    tween = new TweenAlpha(0, SkinnedRectangle::getAlpha(), 1.0f);
    add(tween);

    setVisibility(Visibility::Hidden);
}

void UiStatusBox::show(const std::string &t, bool inf, bool drawNow) {
    text->setString(t);
    setSize(text->getLocalBounds().width + (8 * main->getScaling()), getSize().y);

    infinite = inf;
    clock->restart();
    setVisibility(Visibility::Visible, true);
    if (drawNow) {
        for (int i = 0; i < 30; i++) {
            main->flip();
        }
    }
}

void UiStatusBox::hide() {
    clock->restart();
    infinite = false;
}

void UiStatusBox::onDraw(c2d::Transform &transform, bool draw) {
    if (isVisible() && !infinite && clock->getElapsedTime().asSeconds() > 5) {
        setVisibility(Visibility::Hidden, true);
    }

    SkinnedRectangle::onDraw(transform, draw);
}

UiStatusBox::~UiStatusBox() {
    delete (clock);
}
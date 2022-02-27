//
// Created by cpasjuste on 08/12/18.
//

#include "c2dui.h"

UiStatusBox::UiStatusBox(UiMain *m)
        : SkinnedRectangle(m->getSkin(), {"SKIN_CONFIG", "STATUSBOX"}) {

    main = m;
    clock = new C2DClock();

    icon = new SkinnedRectangle(main->getSkin(), {"SKIN_CONFIG", "STATUSBOX", "ICON"});
    icon->add(new TweenRotation(360, 0, 2, TweenLoop::Loop, TweenState::Playing));
    add(icon);

    text = new Text("TIPS:", (int) (getSize().y * 0.7f), main->getFont());
    text->setOutlineThickness(1.0f * main->getScaling());
    text->setPosition(icon->getSize().x + (4 * main->getScaling()), (getSize().y / 2) + text->getOutlineThickness());
    text->setOrigin(Origin::Left);
    text->setFillColor(SkinnedRectangle::getOutlineColor());
    add(text);

    tween = new TweenAlpha(0, SkinnedRectangle::getAlpha(), 1.0f);
    add(tween);

    setVisibility(Visibility::Hidden);
}

void UiStatusBox::show(const std::string &t, bool inf, bool drawNow) {
    text->setString(t);
    setSize(icon->getLocalBounds().width + text->getLocalBounds().width + (8 * main->getScaling()), getSize().y);

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
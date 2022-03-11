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
    //text->setOutlineThickness(1.0f * main->getScaling());
    text->setOrigin(Origin::Left);
    text->setPosition(6 * main->getScaling(), (getSize().y / 2) + SkinnedRectangle::getOutlineThickness());
    text->setFillColor(SkinnedRectangle::getOutlineColor());
    add(text);

    tween = new TweenAlpha(0, SkinnedRectangle::getAlpha(), 1.0f);
    add(tween);

    setVisibility(Visibility::Hidden);
}

void UiStatusBox::show(const std::string &t, bool inf, bool drawNow) {
    text->setString(t);
    setSize(text->getLocalBounds().width + (12 * main->getScaling()), getSize().y);

    infinite = inf;
    clock->restart();
    setVisibility(Visibility::Visible, true);
    if (drawNow) {
        for (int i = 0; i < 30; i++) {
            main->flip();
        }
    }
}

void UiStatusBox::show(const char *fmt, ...) {
    char msg[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, 512, fmt, args);
    va_end(args);
    show(std::string(msg), false, false);
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
#ifndef __PSP2__ // TODO: fix
    delete (clock);
#endif
}
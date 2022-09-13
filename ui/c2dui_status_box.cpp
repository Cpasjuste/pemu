//
// Created by cpasjuste on 08/12/18.
//

#include "c2dui.h"

UiStatusBox::UiStatusBox(UiMain *m)
        : SkinnedRectangle(m->getSkin(), {"SKIN_CONFIG", "STATUSBOX"}) {
    main = m;
#ifndef __3DS__
    clock = new C2DClock();

    SkinnedRectangle::setCornersRadius(8);
    SkinnedRectangle::setCornerPointCount(8);

    text = new Text("TIPS:", (int) (getSize().y * 0.65f), main->getSkin()->getFont());
    text->setOutlineThickness(1.0f);
    text->setOrigin(Origin::Left);
    text->setPosition(6 * main->getScaling().x, getSize().y / 2);
    text->setFillColor(SkinnedRectangle::getOutlineColor());
    add(text);

    tween = new TweenAlpha(0, SkinnedRectangle::getAlpha(), 1.0f);
    add(tween);
#endif
    setVisibility(Visibility::Hidden);
}

void UiStatusBox::show(const std::string &t) {
#ifndef __3DS__
    text->setString(t);
    text->setPosition(6 * main->getScaling().x, getSize().y / 2);
    setSize(text->getLocalBounds().width + (12 * main->getScaling().x), getSize().y);

    clock->restart();
    setVisibility(Visibility::Visible, true);
#endif
}

void UiStatusBox::show(const char *fmt, ...) {
#ifndef __3DS__
    char msg[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, 512, fmt, args);
    va_end(args);
    show(std::string(msg));
#endif
}

void UiStatusBox::hide() {
#ifndef __3DS__
    clock->restart();
#endif
}

void UiStatusBox::onDraw(c2d::Transform &transform, bool draw) {
#ifndef __3DS__
    if (isVisible() && clock->getElapsedTime().asSeconds() > 5) {
        setVisibility(Visibility::Hidden, true);
    }
#endif
    SkinnedRectangle::onDraw(transform, draw);
}

UiStatusBox::~UiStatusBox() {
#ifndef __3DS__
    delete (clock);
#endif
}
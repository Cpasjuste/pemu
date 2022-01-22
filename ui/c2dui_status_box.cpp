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

    titleText = new Text("WT", (int) (getSize().y * 0.45f), main->getFont());
    titleText->setPosition(icon->getSize().x + 8, 4);
    add(titleText);

    messageText = new Text("WT", (int) (getSize().y * 0.45f), main->getFont());
    messageText->setPosition(titleText->getPosition().x,
                             titleText->getPosition().y + titleText->getSize().y + 2);
    messageText->setSizeMax(getSize().x - icon->getSize().x - 16, 0);
    add(messageText);

    tween = new TweenAlpha(0, 255, 1.0f);
    add(tween);

    setVisibility(Visibility::Hidden);
}

void UiStatusBox::show(const std::string &title, const std::string &message, bool inf, bool drawNow) {

    titleText->setString(title);
    messageText->setString(message);

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
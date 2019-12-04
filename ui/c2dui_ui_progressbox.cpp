//
// Created by cpasjuste on 04/02/18.
//

#include "c2dui.h"

using namespace c2d;
using namespace c2dui;

UIProgressBox::UIProgressBox(UIMain *gui)
        : RectangleShape(Vector2f(gui->getSize().x / 2,
                                  gui->getSize().y / 2)) {

    float w = getSize().x;
    float h = getSize().y;

    gui->getSkin()->loadRectangleShape(this, {"SKIN_CONFIG", "MESSAGEBOX"});

    float margin = UI_MARGIN * gui->getScaling();

    title = new Text("TITLE", C2D_DEFAULT_CHAR_SIZE, gui->getSkin()->getFont());
    title->setPosition(margin, margin + 16);
    title->setSizeMax(w - (margin * 2), 0);
    title->setOutlineThickness(2);
    title->setOutlineColor(Color::Black);
    add(title);

    progress_bg = new RectangleShape(
            FloatRect(margin, h - margin - (h / 6),
                      w - (margin * 2), h / 6));
    Color col = getFillColor();
    col.r = (uint8_t) std::max(0, col.r - 40);
    col.g = (uint8_t) std::max(0, col.g - 40);
    col.b = (uint8_t) std::max(0, col.b - 40);
    progress_bg->setFillColor(col);
    progress_bg->setOutlineColor(getOutlineColor());
    progress_bg->setOutlineThickness(2);
    add(progress_bg);

    progress_fg = new RectangleShape(
            FloatRect(progress_bg->getPosition().x + 1, progress_bg->getPosition().y + 1,
                      2, progress_bg->getSize().y - 2));
    progress_fg->setFillColor(getOutlineColor());
    add(progress_fg);

    message = new Text("MESSAGE", (unsigned int) gui->getFontSize(), gui->getSkin()->getFont());
    message->setPosition(margin, progress_bg->getPosition().y - C2D_DEFAULT_CHAR_SIZE - 16);
    message->setSizeMax(w - (margin * 2), 0);
    message->setOutlineThickness(2);
    message->setOutlineColor(Color::Black);
    add(message);

    setVisibility(Visibility::Hidden);
}

void UIProgressBox::setTitle(std::string title) {

    this->title->setString(title);
}

void UIProgressBox::setProgress(float progress) {

    float width = progress * (progress_bg->getSize().x - 2);
    progress_fg->setSize(
            std::min(width, progress_bg->getSize().x - 2),
            progress_fg->getSize().y);
}

void UIProgressBox::setMessage(std::string message) {

    this->message->setString(message);
}

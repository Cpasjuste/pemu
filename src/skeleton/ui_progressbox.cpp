//
// Created by cpasjuste on 04/02/18.
//

#include "pemu.h"

UIProgressBox::UIProgressBox(UiMain *ui)
        : SkinnedRectangle(ui, {"SKIN_CONFIG", "MESSAGEBOX"}) {
    float w = getSize().x;
    float h = getSize().y;

    float font_size = h / 15.0f;
    float margin_x = font_size * ui->getScaling().x;
    float margin_y = font_size * ui->getScaling().y;

    m_title = new Text("TITLE", (unsigned int) (font_size * 1.3f), ui->getSkin()->getFont());
    m_title->setPosition(margin_x, margin_y);
    m_title->setSizeMax(w - (margin_x * 2), 0);
    m_title->setOutlineThickness(2);
    m_title->setOutlineColor(Color::Black);
    add(m_title);

    m_progress_bg = new RectangleShape({margin_x, h - margin_y - (h / 6), w - (margin_x * 2), h / 6});
    Color col = getFillColor();
    col.r = (uint8_t) std::max(0, col.r - 40);
    col.g = (uint8_t) std::max(0, col.g - 40);
    col.b = (uint8_t) std::max(0, col.b - 40);
    m_progress_bg->setFillColor(col);
    m_progress_bg->setOutlineColor(getOutlineColor());
    m_progress_bg->setOutlineThickness(2);
    add(m_progress_bg);

    m_progress_fg = new RectangleShape(
            FloatRect(m_progress_bg->getPosition().x + 1, m_progress_bg->getPosition().y + 1,
                      2, m_progress_bg->getSize().y - 2));
    m_progress_fg->setFillColor(getOutlineColor());
    add(m_progress_fg);

    m_message = new Text("MESSAGE", (unsigned int) font_size, ui->getSkin()->getFont());
    m_message->setOrigin(Origin::BottomLeft);
    m_message->setPosition(margin_x, m_progress_bg->getPosition().y - margin_y);
    m_message->setSizeMax(w - (margin_x * 2), 0);
    m_message->setOutlineThickness(2);
    m_message->setOutlineColor(Color::Black);
    add(m_message);

    setVisibility(Visibility::Hidden);
}

void UIProgressBox::setTitle(const std::string &title) {
    m_title->setString(title);
}

void UIProgressBox::setProgress(float progress) {
    float width = progress * (m_progress_bg->getSize().x - 2);
    m_progress_fg->setSize(
            std::min(width, m_progress_bg->getSize().x - 2),
            m_progress_fg->getSize().y);
}

void UIProgressBox::setMessage(const std::string &message) {
    m_message->setString(message);
}

c2d::Text *UIProgressBox::getTitleText() {
    return m_title;
}

c2d::Text *UIProgressBox::getMessageText() {
    return m_message;
}

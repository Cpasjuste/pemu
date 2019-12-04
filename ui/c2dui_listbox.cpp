//
// Created by cpasjuste on 05/01/18.
//

#include "c2dui.h"
#include "c2dui_listbox.h"

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

UIListBoxLine::UIListBoxLine(
        const FloatRect &rect, const std::string &str,
        Font *font, unsigned int fontSize, Texture *i, bool use_ic) : RectangleShape(rect) {

    //printf("ListBoxLine(%p)\n", this);

    icon = i;
    use_icons = use_ic;
    text = new Text(str, fontSize, font);
    text->setOutlineColor(Color::Black);
    text->setOrigin(Origin::Left);

    setFillColor(Color::Transparent);

    if (use_icons) {
        // add iconRect
        iconRect = new RectangleShape(Vector2f(32, 32));
        iconRect->setPosition(Vector2f(1, 1));
        iconRect->setFillColor(Color::Transparent);
        iconRect->setOutlineThickness(1);
        add(iconRect);
        // icon added in ListBox::setSelection (setIcon)
        // set text
        text->setPosition(iconRect->getSize().x + 8, getSize().y / 2);
        text->setSizeMax(getSize().x - fontSize - iconRect->getSize().x, 0);
    } else {
        text->setPosition(2, getSize().y / 2);
        text->setSizeMax(getSize().x - (fontSize + 8), 0);
    }

    add(text);
}

void UIListBoxLine::setSize(const Vector2f &size) {
    UIListBoxLine::setSize(size.x, size.y);
}

void UIListBoxLine::setSize(float width, float height) {
    RectangleShape::setSize(width, height);
    text->setSizeMax(getSize().x - 8, 0);
}

void UIListBoxLine::setString(const std::string &string) {
    text->setString(string);
}

void UIListBoxLine::setColor(const Color &color) {

    text->setFillColor(color);
    if (iconRect) {
        iconRect->setOutlineColor(color);
    }
    if (icon) {
        icon->setOutlineColor(color);
    }
}

void UIListBoxLine::setIcon(Texture *i) {

    if (use_icons) {
        if (icon) {
            remove(icon);
        }
        icon = i;
        if (icon) {
            if (icon->available) {
                icon->setOutlineThickness(1);
                icon->setOrigin(iconRect->getOrigin());
                icon->setPosition(iconRect->getPosition());
                if (icon->getLocalBounds().width != 32 || icon->getLocalBounds().height != 32) {
                    printf("ListBoxLine: icon size != 32, scaling icon...\n");
                    float scaling = std::min(
                            iconRect->getSize().y / icon->getLocalBounds().width,
                            iconRect->getSize().y / icon->getLocalBounds().height);
                    icon->setScale(scaling, scaling);
                }
            }
            add(icon);
            icon->setVisibility(icon->available ? Visibility::Visible : Visibility::Hidden);
            iconRect->setVisibility(icon->available ? Visibility::Hidden : Visibility::Visible);
        } else {
            iconRect->setVisibility(Visibility::Visible);
        }
    }
}

Text *UIListBoxLine::getText() {
    return text;
}

UIListBoxLine::~UIListBoxLine() {
    //printf("~ListBoxLine(%p)\n", this);
}

UIListBox::UIListBox(Font *font, int fontSize, const FloatRect &rect,
                     const std::vector<ss_api::Game> &g, bool useIcons) : RectangleShape(rect) {

    //printf("ListBox(%p)\n", this);
    setGames(games);
    init(font, fontSize, useIcons);
    setSelection(0);
};

void UIListBox::init(Font *font, int fontSize, bool useIcons) {

    use_icons = useIcons;
    // set default bg colors
    setFillColor(Color::GrayLight);

    // calculate number of lines shown
    if (use_icons) {
        line_height = 34; // 32px + 2px margin
    } else {
        line_height = (float) fontSize + 2;
    }
    max_lines = (int) (getSize().y / line_height);
    if ((float) max_lines * line_height < getSize().y) {
        line_height = getSize().y / (float) max_lines;
    }

    // add selection rectangle (highlight)
    highlight = new RectangleShape(Vector2f(getSize().x - 2, line_height - 2));
    add(highlight);

    // add lines of text
    for (unsigned int i = 0; i < (unsigned int) max_lines; i++) {
        FloatRect r = {1, (line_height * i) + 1, getSize().x - 2, line_height - 2};
        Texture *icon = nullptr;
        /*
         * TODO: fix icons
        if (use_icons) {
            icon = files.size() > i ? files[i]->icon : nullptr;
        }
        */
        auto line = new UIListBoxLine(r, "", font, (unsigned int) fontSize, icon, use_icons);
        lines.push_back(line);
        add(line);
    }
}

void UIListBox::updateLines() {

    for (unsigned int i = 0; i < (unsigned int) max_lines; i++) {

        if (start_index + i >= games.size()) {
            lines[i]->setVisibility(Visibility::Hidden);
        } else {
            // set file
            Game game = games[start_index + i];
            lines[i]->setVisibility(Visibility::Visible);
            lines[i]->setString(game.getName().text);
            // TODO: ICON
            //lines[i]->setIcon(file->icon);
            lines[i]->setColor(game.available ? colorAvailable : colorMissing);
            // set highlight position and color
            if ((int) i == highlight_index) {
                highlight->setPosition(lines[i]->getPosition());
                Color color = highlight_use_files_color ?
                              lines[i]->getText()->getFillColor() : highlight->getFillColor();
                color.a = highlight->getAlpha();
                highlight->setFillColor(color);
                color = highlight_use_files_color ?
                        lines[i]->getText()->getFillColor() : highlight->getOutlineColor();
                //color.a = highlight->getAlpha();
                highlight->setOutlineColor(color);
            }
        }
    }

    if (games.empty()) {
        highlight->setVisibility(Visibility::Hidden, false);
    } else {
        if (use_highlight) {
            highlight->setVisibility(Visibility::Visible, false);
        }
    }
}

void UIListBox::up() {

    file_index--;
    if (file_index < 0) {
        file_index = (int) games.size() - 1;
        start_index = (int) games.size() - max_lines;
        highlight_index = max_lines - 1;
    } else {
        if (highlight_index <= 0) {
            start_index--;
        } else {
            highlight_index--;
        }
    }
    updateLines();
}

void UIListBox::down() {

    file_index++;
    if (file_index >= (int) games.size()) {
        file_index = 0;
        start_index = 0;
        highlight_index = 0;
    } else {
        if (highlight_index >= max_lines - 1) {
            start_index++;
        } else {
            highlight_index++;
        }
    }
    updateLines();
}

void UIListBox::setSelection(int new_index) {

    file_index = new_index;
    int page = file_index / max_lines;
    start_index = page * max_lines;
    highlight_index = file_index - start_index;
    updateLines();
}

void UIListBox::setSize(const Vector2f &size) {
    UIListBox::setSize(size.x, size.y);
}

void UIListBox::setSize(float width, float height) {
    RectangleShape::setSize(width, height);
    highlight->setSize(width, highlight->getSize().y);
    for (auto &line : lines) {
        line->setSize(width, line->getSize().y);
    }
}

void UIListBox::setGames(const std::vector<ss_api::Game> &g) {

    games = g;

    if (!lines.empty()) {
        setSelection(0);
    }
}

std::vector<ss_api::Game> UIListBox::getGames() {
    return games;
}

Game UIListBox::getSelection() {
    if (!games.empty() && games.size() > (unsigned int) file_index) {
        return games[file_index];
    }
    return Game();
}

std::vector<UIListBoxLine *> UIListBox::getLines() {
    return lines;
}

void UIListBox::setTextOutlineColor(const Color &color) {
    for (auto &line : lines) {
        line->getText()->setOutlineColor(color);
    }
}

void UIListBox::setTextOutlineThickness(float thickness) {
    for (auto &line : lines) {
        line->getText()->setOutlineThickness(thickness);
    }
}

void UIListBox::setHighlightEnabled(bool enable) {
    use_highlight = enable;
    highlight->setVisibility(enable ? Visibility::Visible
                                    : Visibility::Hidden);
}

void UIListBox::setHighlightUseFileColor(bool enable) {
    highlight_use_files_color = enable;
    setSelection(file_index);
}

int UIListBox::getIndex() {
    return file_index;
}

int UIListBox::getMaxLines() {
    return max_lines;
}

RectangleShape *UIListBox::getHighlight() {
    return highlight;
}

UIListBox::~UIListBox() {
    //printf("~ListBox(%p)\n", this);
}

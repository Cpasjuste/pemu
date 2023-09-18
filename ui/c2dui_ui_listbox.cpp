//
// Created by cpasjuste on 05/01/18.
//

#include "c2dui.h"

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

UIListBoxLine::UIListBoxLine(
        const FloatRect &rect, const std::string &str,
        Font *font, unsigned int fontSize, const c2d::Vector2f &scaling, Texture *i, bool use_ic) : RectangleShape(
        rect) {
    //printf("ListBoxLine(%p)\n", this);
    icon = i;
    use_icons = use_ic;
    text = new Text(str, fontSize, font);
    text->setOutlineColor(Color::Black);
    text->setOrigin(Origin::Left);

    UIListBoxLine::setFillColor(Color::Transparent);

    if (use_icons) {
        // add iconRect
        iconRect = new RectangleShape(Vector2f(32, 32));
        iconRect->setPosition(Vector2f(1, 1));
        iconRect->setFillColor(Color::Transparent);
        iconRect->setOutlineThickness(1);
        UIListBoxLine::add(iconRect);
        // icon added in ListBox::setSelection (setIcon)
        // set text
        text->setPosition(iconRect->getSize().x + 8, UIListBoxLine::getSize().y / 2);
        text->setSizeMax(UIListBoxLine::getSize().x - (float) fontSize - iconRect->getSize().x, 0);
    } else {
        text->setPosition(2 * scaling.x, UIListBoxLine::getSize().y / 2);
        text->setSizeMax(UIListBoxLine::getSize().x - text->getSize().x - (2 * scaling.x) - 2, 0);
    }

    UIListBoxLine::add(text);
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
    if (iconRect != nullptr) {
        iconRect->setOutlineColor(color);
    }
    if (icon != nullptr) {
        icon->setOutlineColor(color);
    }
}

void UIListBoxLine::setIcon(Texture *i) {
    if (use_icons) {
        if (icon != nullptr) {
            remove(icon);
        }
        icon = i;
        if (icon != nullptr) {
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

UIListBox::UIListBox(UiMain *_ui, Font *font, int fontSize, const FloatRect &rect,
                     const std::vector<ss_api::Game> &g) : RectangleShape(rect) {
    //printf("ListBox(%p)\n", this);
    ui = _ui;
    setGames(g);
    init(font, fontSize);
    setSelection(0);
};

void UIListBox::init(Font *font, int fontSize) {
    // set default bg colors
    setFillColor(Color::GrayLight);

    // calculate number of lines shown
    line_height = (float) fontSize + (2 * ui->getScaling().y);
    max_lines = (int) (getSize().y / line_height);
    if ((float) max_lines * line_height < getSize().y) {
        line_height = getSize().y / (float) max_lines;
    }

    // add selection rectangle (highlight)
    highlight = new RectangleShape(Vector2f(getSize().x - 2, line_height));
    add(highlight);

    // add lines of text
    for (unsigned int i = 0; i < (unsigned int) max_lines; i++) {
        FloatRect r = {0, line_height * (float) i, getSize().x, line_height};
        Texture *icon = nullptr;
        /* TODO: add back icon support ?
        if (use_icons) {
            icon = files.size() > i ? files[i]->icon : nullptr;
        }
        */
        auto *line = new UIListBoxLine(r, "W", font, (unsigned int) fontSize, ui->getScaling(), icon);
        lines.push_back(line);
        add(line);
    }
}

void UIListBox::updateLines() {
    bool useZipName = ui->getConfig()->getOption(PEMUConfig::Id::GUI_SHOW_ZIP_NAMES)->getInteger();

    for (unsigned int i = 0; i < (unsigned int) max_lines; i++) {
        if (file_index + i >= games.size()) {
            lines[i]->setVisibility(Visibility::Hidden);
        } else {
            // set file
            Game game = games[file_index + i];
            lines[i]->setVisibility(Visibility::Visible);
            lines[i]->setString(useZipName ? Utility::removeExt(game.path) : game.name);
            // TODO: ICON
            //lines[i]->setIcon(file->icon);
            lines[i]->setColor(game.available ? colorAvailable : colorMissing);
            // set highlight position and color
            if ((int) i == highlight_index) {
                highlight->setPosition(lines[i]->getPosition());
                highlight->move(1, 0);
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
    int index = file_index + highlight_index;
    int middle = max_lines / 2;

    if (highlight_index <= middle && index - middle > 0) {
        file_index--;
    } else {
        highlight_index--;
    }

    if (highlight_index < 0) {
        highlight_index = (int) games.size() < max_lines - 1 ? (int) games.size() - 1 : max_lines - 1;
        file_index = ((int) games.size() - 1) - highlight_index;
    }

    updateLines();
}

void UIListBox::down() {
    int index = file_index + highlight_index;
    int middle = max_lines / 2;

    if (highlight_index >= middle && index + (max_lines - middle) < (int) games.size()) {
        file_index++;
    } else {
        highlight_index++;
    }

    if (highlight_index >= max_lines || file_index + highlight_index >= (int) games.size()) {
        file_index = 0;
        highlight_index = 0;
    }

    updateLines();
}

void UIListBox::setSelection(int new_index) {
    if (new_index < max_lines / 2) {
        file_index = 0;
        highlight_index = 0;
    } else if (new_index > (int) games.size() - max_lines / 2) {
        highlight_index = max_lines / 2;
        file_index = (int) games.size() - 1 - highlight_index;
        if (highlight_index >= (int) games.size()) {
            highlight_index = (int) games.size() - 1;
            file_index = 0;
        }
    } else {
        highlight_index = max_lines / 2;
        file_index = new_index - highlight_index;
    }

    updateLines();
}

void UIListBox::setSize(const Vector2f &size) {
    UIListBox::setSize(size.x, size.y);
}

void UIListBox::setSize(float width, float height) {
    RectangleShape::setSize(width, height);
    highlight->setSize(width, highlight->getSize().y);
    for (auto &line: lines) {
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
    if (!games.empty() && games.size() > (size_t) file_index + highlight_index) {
        return games[file_index + highlight_index];
    }
    return {};
}

std::vector<UIListBoxLine *> UIListBox::getLines() {
    return lines;
}

void UIListBox::setTextOutlineColor(const Color &color) {
    for (auto &line: lines) {
        line->getText()->setOutlineColor(color);
    }
}

void UIListBox::setTextOutlineThickness(float thickness) {
    for (auto &line: lines) {
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
    return file_index + highlight_index;
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

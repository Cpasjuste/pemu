//
// Created by cpasjuste on 05/01/18.
//

#ifndef C2DUI_LISTBOX_H
#define C2DUI_LISTBOX_H

#include "cross2d/skeleton/sfml/RectangleShape.hpp"
#include "cross2d/skeleton/sfml/Text.hpp"
#include "cross2d/skeleton/io.h"
#include "ss_api.h"

namespace c2dui {

    class UIListBoxLine : public c2d::RectangleShape {

    public:

        UIListBoxLine(const c2d::FloatRect &rect, const std::string &str, c2d::Font *font,
                      unsigned int fontSize, c2d::Texture *icon = nullptr, bool use_icons = false);

        ~UIListBoxLine() override;

        void setSize(const c2d::Vector2f &size) override;

        void setSize(float width, float height) override;

        void setString(const std::string &string);

        void setColor(const c2d::Color &color);

        void setIcon(c2d::Texture *icon);

        c2d::Text *getText();

    private:

        RectangleShape *iconRect = nullptr;
        c2d::Texture *icon = nullptr;
        c2d::Text *text = nullptr;
        bool use_icons = false;
    };

    class UIListBox : public c2d::RectangleShape {

    public:

        UIListBox(c2d::Font *font, int fontSize, const c2d::FloatRect &rect,
                  const std::vector<ss_api::Game> &games, bool useIcons = false);

        ~UIListBox() override;

        void setGames(const std::vector<ss_api::Game> &games);

        void setSelection(int index);

        void up();

        void down();

        void setSize(const c2d::Vector2f &size) override;

        void setSize(float width, float height) override;

        void setTextOutlineColor(const c2d::Color &color);

        void setTextOutlineThickness(float thickness);

        void setHighlightEnabled(bool enable);

        void setHighlightUseFileColor(bool enable);

        RectangleShape *getHighlight();

        int getMaxLines();

        std::vector<UIListBoxLine *> getLines();

        std::vector<ss_api::Game> getGames();

        ss_api::Game getSelection();

        int getIndex();

        c2d::Color colorMissing;
        c2d::Color colorAvailable;

    private:

        void init(c2d::Font *font, int fontSize, bool useIcons);

        void updateLines();

        std::vector<ss_api::Game> games;
        std::vector<UIListBoxLine *> lines;
        RectangleShape *highlight;
        float line_height;
        int max_lines;
        int file_index = 0;
        int start_index = 0;
        int highlight_index = 0;
        bool use_icons = false;
        bool use_highlight = true;
        bool highlight_use_files_color = false;
    };
}

#endif //C2DUI_LISTBOX_H

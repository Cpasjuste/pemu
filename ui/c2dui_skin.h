//
// Created by cpasjuste on 23/11/16.
//

#ifndef C2DUI_SKIN_H_
#define C2DUI_SKIN_H_

#include <string>
#include <vector>

namespace c2dui {

    class Skin {

    public:

        struct RectangleShapeGroup {
            c2d::FloatRect rect;
            c2d::Origin origin;
            std::string texture;
            int filtering = 1;
            c2d::Color color;
            c2d::Color outlineColor;
            float outlineSize = 0;
            c2d::Vector2f scaling = {1, 1};
            bool available = false;
        };

        struct TextGroup {
            std::string text;
            unsigned int size;
            c2d::FloatRect rect;
            c2d::Origin origin;
            c2d::Color color;
            c2d::Color outlineColor;
            c2d::Text::Overflow overflow;
            float outlineSize = 0;
            c2d::Vector2f scaling = {1, 1};
            bool available = false;
        };

        struct Button {
            Button(int i, const std::string &n) {
                id = i;
                name = n;
            }

            std::string name;
            c2d::Texture *texture = nullptr;
            int id = -1;
        };

        Skin(UiMain *ui);

        ~Skin();

        c2d::config::Config *getConfig();

        bool loadRectangleShape(c2d::RectangleShape *shape, const std::vector<std::string> &tree,
                                bool textureUseFillColors = false);

        RectangleShapeGroup getRectangleShape(const std::vector<std::string> &tree);

        bool loadText(c2d::Text *text, const std::vector<std::string> &tree);

        TextGroup getText(const std::vector<std::string> &tree);

        Button *getButton(int id);

        c2d::Font *getFont();

        std::string path;
        c2d::Font *font = nullptr;
        bool font_available = true;
        char *font_data = nullptr;
        std::vector<Button> buttons;

    private:

        c2d::config::Group createRectangleShapeGroup(const std::string &name);

        c2d::config::Group createTextGroup(const std::string &name, int size = 0);

        UiMain *ui = nullptr;
        c2d::config::Config *config = nullptr;
        c2d::Vector2f global_scaling = {1.0f, 1.0f};
    };
}

#endif //C2DUI_SKIN_H_

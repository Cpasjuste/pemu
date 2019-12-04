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
            int outlineSize;
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
            int outlineSize;
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

        Skin(UIMain *ui, const std::vector<Button> &buttons);

        ~Skin();

        c2d::config::Config *getConfig();

        void loadRectangleShape(c2d::RectangleShape *shape, const std::vector<std::string> &tree);

        RectangleShapeGroup getRectangleShape(const std::vector<std::string> &tree);

        void loadText(c2d::Text *text, const std::vector<std::string> &tree);

        TextGroup getText(const std::vector<std::string> &tree);

        Button *getButton(int id);

        c2d::Font *getFont();

        char *getZippedData(const std::string &path, const std::string &name, int *size = nullptr);

        std::string path;
        c2d::Font *font = nullptr;
        bool font_available = true;
        char *font_data = nullptr;
        std::vector<Button> buttons;

    private:

        c2d::config::Group createRectangleShapeGroup(const std::string &name,
                                                     const c2d::FloatRect &rect,
                                                     const c2d::Origin &origin,
                                                     const std::string &texture,
                                                     const c2d::Color &color,
                                                     const c2d::Color &outlineColor, int outlineSize,
                                                     c2d::Vector2f scale = {1, 1});

        c2d::config::Group createTextGroup(const std::string &name,
                                           int size,
                                           const c2d::FloatRect &rect,
                                           const c2d::Origin &origin,
                                           const c2d::Color &color,
                                           const c2d::Color &outlineColor, int outlineSize,
                                           const c2d::Text::Overflow &overflow = c2d::Text::Overflow::Clamp,
                                           c2d::Vector2f scale = {1, 1});

        UIMain *ui = nullptr;
        c2d::config::Config *config = nullptr;
        bool useZippedSkin = false;
        c2d::Vector2f global_scaling = {1.0f, 1.0f};
    };
}

#endif //C2DUI_SKIN_H_

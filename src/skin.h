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
            enum TextureRatio {
                Free, KeepWidth, KeepHeight
            };

            c2d::FloatRect rect;
            c2d::Origin origin;
            std::string texture;
            TextureRatio textureRatio = Free;
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

        explicit Skin(UiMain *ui);

        ~Skin();

        c2d::config::Config *getConfig();

        std::array<std::string, 2> getPaths() { return mSkinPath; };

        c2d::config::Group *getGroup(const std::vector<std::string> &tree);

        bool loadRectangleShape(c2d::RectangleShape *shape, const std::vector<std::string> &tree,
                                bool textureUseFillColors = false);

        RectangleShapeGroup getRectangleShape(const std::vector<std::string> &tree);

        bool loadText(c2d::Text *text, const std::vector<std::string> &tree);

        TextGroup getText(const std::vector<std::string> &tree);

        Button *getButton(int id);

        c2d::Font *getFont();

        c2d::Vector2f getScaling() { return mScaling; };

        float getFontScaling() { return mFontScaling; };

        c2d::FloatRect getRectangle(c2d::config::Group *group);

        c2d::Color getColor(c2d::config::Group *group, const std::string &name);

        c2d::Color getFillColor(c2d::config::Group *group);

        c2d::Color getOutlineColor(c2d::config::Group *group);

        float getOutlineThickness(c2d::config::Group *group);

        c2d::Vector2f getScale(c2d::config::Group *group);

        c2d::Origin getOrigin(c2d::config::Group *group);

        unsigned int getCharacterSize(c2d::config::Group *group);

        std::string getString(c2d::config::Group *group);

        c2d::Text::Overflow getOverflow(c2d::config::Group *group);

    private:
        c2d::config::Group createRectangleShapeGroup(const std::string &name);

        c2d::config::Group createTextGroup(const std::string &name, int size = 0);

        UiMain *pMain;
        c2d::config::Config *pConfig;
        c2d::Vector2f mScaling = {1.0f, 1.0f};
        float mFontScaling = 1.0f;
        c2d::Font *pFont = nullptr;
        std::vector<Button> mButtons;
        std::array<std::string, 2> mSkinPath;
    };
}

#endif //C2DUI_SKIN_H_

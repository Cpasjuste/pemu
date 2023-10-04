//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKIN_H
#define RR_LAUNCHER_SKIN_H

#include <tinyxml2.h>
#include "cross2d/c2d.h"
#include "skinned_widget.h"
#include "skinned_rectangle.h"
#include "skinned_text.h"
#include "skinned_sprite.h"
#include "skinned_listbox.h"
#include "skinned_progress_bar.h"

#ifndef NDEBUG
#define rr_debug(...) \
    printf("%s:%i %s(): ", __FILE__, __LINE__, __func__); \
    printf(__VA_ARGS__); \
    printf("\n")
#else
#define rr_debug(...) ((void)0)
#endif

#define rr_info(...) \
    printf("\033[0;32m"); \
    printf("%s:%i %s(): ", __FILE__, __LINE__, __func__); \
    printf(__VA_ARGS__); \
    printf("\n\033[0m")

#define rr_error(...) \
    fprintf(stderr, "%s:%i %s(): ", __FILE__, __LINE__, __func__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n")

namespace RRLauncher {
    class App;

    class Skin : public c2d::C2DRenderer {
    public:
        struct Color {
            std::string name;
            c2d::Color object;
        };

        struct Font {
            std::string name;
            c2d::Font *object;
        };

        struct Texture {
            std::string name;
            c2d::Texture *object;
        };

        class Widget {
        public:
            Widget() { doc = new tinyxml2::XMLDocument(); };

            ~Widget() { delete doc; }

            std::string name;
            std::string path;
            tinyxml2::XMLDocument *doc = nullptr;
            tinyxml2::XMLNode *node = nullptr;
        };

        explicit Skin(const c2d::Vector2f &size);

        ~Skin() override;

        virtual std::string getName() {
            return m_name;
        }

        virtual void load();

        virtual std::string getVar(const std::string &var, const std::string &data = {}, int index = -1) {
            return {};
        };

        virtual size_t getVarInt(const std::string &var, const std::string &data = {}, int index = -1) {
            return 0;
        };

        virtual size_t getDataCount(const std::string &name) { return 0; };

        virtual void setDataIndex(const std::string &name, size_t index) {};

        virtual void setDataFilter(const std::string &name, const std::string &filter) {};

        virtual void updateData(const std::string &name) {};

        virtual c2d::Color getColor(tinyxml2::XMLElement *element);

        virtual c2d::Font *getFont(tinyxml2::XMLElement *element);

        virtual c2d::Texture *getTexture(tinyxml2::XMLElement *element);

    private:
        void loadWidgets();

        std::string m_path;
        std::string m_name;
        tinyxml2::XMLDocument m_doc;
        tinyxml2::XMLNode *m_root;
        std::vector<Color> m_colors;
        std::vector<Font> m_fonts;
        std::vector<Texture> m_textures;
    };
}

#endif //RR_LAUNCHER_SKIN_H

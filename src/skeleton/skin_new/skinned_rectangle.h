//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKINNED_RECTANGLE_H
#define RR_LAUNCHER_SKINNED_RECTANGLE_H

namespace RRLauncher {
    class Skin;

    class SkinnedRectangle : public c2d::RectangleShape, public SkinnedWidget {
    public:
        explicit SkinnedRectangle(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent, bool load = true);

    protected:
        void loadAttributes(tinyxml2::XMLNode *node) override;

        bool onInput(c2d::Input::Player *players) override;
    };
};

#endif //RR_LAUNCHER_SKINNED_RECTANGLE_H

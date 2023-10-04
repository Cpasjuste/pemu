//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKINNED_SPRITE_H
#define RR_LAUNCHER_SKINNED_SPRITE_H

namespace RRLauncher {
    class Skin;

    class SkinnedSprite : public c2d::Sprite, public SkinnedWidget {
    public:
        explicit SkinnedSprite(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent);

        ~SkinnedSprite() override;

    private:
        void loadAttributes(tinyxml2::XMLNode *node) override;

        bool onInput(c2d::Input::Player *players) override;

        void onUpdate() override;

        VariableString m_var_str;
        c2d::C2DTexture *m_texture = nullptr;
        std::string m_texture_path;
    };
}

#endif //RR_LAUNCHER_SKINNED_SPRITE_H

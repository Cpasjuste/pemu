//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKINNED_PROGRESS_BAR_H
#define RR_LAUNCHER_SKINNED_PROGRESS_BAR_H

namespace RRLauncher {
    class Skin;

    class SkinnedProgressBar : public SkinnedRectangle {
    public:
        enum Direction {
            Left,
            Right,
            Up,
            Down
        };

        explicit SkinnedProgressBar(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent, bool load = true);

    private:
        void load() override;

        void loadAttributes(tinyxml2::XMLNode *node) override;

        void onUpdate() override;

        std::string m_var_value;
        std::string m_var_value_max;
        Direction m_direction = Direction::Right;
        c2d::Vector2f m_size_max;
        bool m_auto_hide = false;
    };
}

#endif //RR_LAUNCHER_SKINNED_PROGRESS_BAR_H

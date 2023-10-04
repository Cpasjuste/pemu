//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKINNED_TEXT_H
#define RR_LAUNCHER_SKINNED_TEXT_H

#include "variable_string.h"

namespace RRLauncher {
    class Skin;

    class SkinnedText : public c2d::Text, public SkinnedWidget {
    public:
        explicit SkinnedText(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent);

    private:
        void loadAttributes(tinyxml2::XMLNode *node) override;

        bool onInput(c2d::Input::Player *players) override;

        void onUpdate() override;

        VariableString m_var_str;
    };
};

#endif //RR_LAUNCHER_SKINNED_TEXT_H
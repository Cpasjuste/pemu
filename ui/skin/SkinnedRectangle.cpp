//
// Created by cpasjuste on 13/12/2021.
//

#include "c2dui.h"
#include "SkinnedRectangle.h"

SkinnedRectangle::SkinnedRectangle(c2dui::Skin *skin, const std::vector<std::string> &cfgTree)
        : RectangleShape({0, 0}) {
    skin->loadRectangleShape(this, cfgTree);
}

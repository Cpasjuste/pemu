//
// Created by cpasjuste on 13/12/2021.
//

#include "c2dui.h"
#include "SkinnedRectangle.h"

SkinnedRectangle::SkinnedRectangle(c2dui::Skin *skin, const std::vector<std::string> &cfgTree)
        : RectangleShape({0, 0}) {

    printf("SkinnedRectangle: ");
    for (const auto &i: cfgTree) {
        printf("=> %s ", i.c_str());
    }
    printf("\n");

    if (!skin->loadRectangleShape(this, cfgTree)) {
        printf("SkinnedRectangle: tree not found in config file...\n");
        available = false;
        return;
    }

    available = true;
}

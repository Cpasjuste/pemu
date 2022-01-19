//
// Created by cpasjuste on 13/12/2021.
//

#include "c2dui.h"
#include "SkinnedText.h"

SkinnedText::SkinnedText(c2dui::Skin *skin, const std::vector<std::string> &cfgTree) : Text() {
    if (!skin->loadText(this, cfgTree)) {
        printf("SkinnedText: could not load %s\n", cfgTree.at(0).c_str());
        available = false;
        return;
    }

    available = true;
}

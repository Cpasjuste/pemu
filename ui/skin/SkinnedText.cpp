//
// Created by cpasjuste on 13/12/2021.
//

#include "c2dui.h"
#include "SkinnedText.h"

SkinnedText::SkinnedText(c2dui::Skin *skin, const std::vector<std::string> &cfgTree) : Text() {

    printf("SkinnedText: ");
    for (const auto & i : cfgTree) {
        printf("=> %s ", i.c_str());
    }
    printf("\n");

    if (!skin->loadText(this, cfgTree)) {
        printf("SkinnedText: tree not found in config file...\n");
        available = false;
        return;
    }

    available = true;
}

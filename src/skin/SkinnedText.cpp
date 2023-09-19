//
// Created by cpasjuste on 13/12/2021.
//

#include "pemu.h"

SkinnedText::SkinnedText(UiMain *main, const std::vector<std::string> &cfgTree) : Text() {
    pMain = main;
    pIo = pMain->getIo();
    pSkin = pMain->getSkin();
    mTree = cfgTree;
    available = load();
}

bool SkinnedText::load() {
    auto group = pSkin->getGroup(mTree);
    if (!group) {
        return false;
    }

    // load text rectangle
    auto rect = pSkin->getRectangle(group);
    if (rect.width <= 0 || rect.height <= 0) {
        printf("SkinnedText::load: width or height is equal to zero, skipping\n");
        return false;
    }
    setPosition(rect.left, rect.top);
    setSizeMax(rect.width, rect.height);

    // load text character size
    auto size = pSkin->getCharacterSize(group);
    if (size > 0) {
        setCharacterSize(size);
    } else {
        return false;
    }

    // set text font
    setFont(pSkin->getFont());

    // set text string
    setString(pSkin->getString(group));

    // load text color
    setFillColor(pSkin->getFillColor(group));

    // load text outline color
    setOutlineColor(pSkin->getOutlineColor(group));

    // load text outline thickness
    setOutlineThickness(pSkin->getOutlineThickness(group));

    // load text origin
    setOrigin(pSkin->getOrigin(group));

    // load text overflow
    setOverflow(pSkin->getOverflow(group));

    return true;
}

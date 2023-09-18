//
// Created by cpasjuste on 13/12/2021.
//

#include "c2dui.h"

SkinnedRectangle::SkinnedRectangle(UiMain *main, const std::vector<std::string> &cfgTree)
        : RectangleShape({0, 0}) {
    pMain = main;
    pIo = pMain->getIo();
    pSkin = pMain->getSkin();
    mTree = cfgTree;
    available = load();
}

bool SkinnedRectangle::load() {
    auto group = pSkin->getGroup(mTree);
    if (!group) {
        return false;
    }

    // load shape rectangle
    auto rect = pSkin->getRectangle(group);
    if (rect.width <= 0 || rect.height <= 0) {
        printf("SkinnedRectangle::load: width or height is equal to zero, skipping\n");
        return false;
    }
    setPosition(rect.left, rect.top);
    setSize(rect.width, rect.height);

    // load texture if any
    auto option = group->getOption("texture");
    if (option && !option->getString().empty()) {
        for (const auto &skinPath: pSkin->getPaths()) {
            std::string texPath = skinPath + option->getString();
            if (!pTex || pTex->m_path != texPath) {
                if (pIo->exist(texPath)) {
                    printf("SkinnedRectangle::load: %s\n", texPath.c_str());
                    pTex = new C2DTexture(texPath);
                    if (pTex->available) {
                        // set texture ratio
                        float scaling = std::min(
                                getSize().x / pTex->getSize().x,
                                getSize().y / pTex->getSize().y);
                        pTex->setScale(scaling, scaling);
                        // set texture filtering
                        option = group->getOption("texture_filtering");
                        if (option) {
                            pTex->setFilter((Texture::Filter) option->getInteger());
                        }
                        // set texture color
                        pTex->setFillColor(pSkin->getColor(group, "texture_color"));
                        // add to the shape
                        add(pTex);
                    } else {
                        delete (pTex);
                    }
                    break;
                }
            } else {
                delete (pTex);
            }
        }
    }

    // load shape color
    setFillColor(pSkin->getFillColor(group));

    // load shape outline color
    setOutlineColor(pSkin->getOutlineColor(group));

    // load shape outline thickness
    setOutlineThickness(pSkin->getOutlineThickness(group));

    // load shape origin
    setOrigin(pSkin->getOrigin(group));

    return true;
}

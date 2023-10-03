//
// Created by cpasjuste on 13/12/2021.
//

#ifndef PEMU_SKINNEDRECTANGLE_H
#define PEMU_SKINNEDRECTANGLE_H

class SkinnedRectangle : public c2d::RectangleShape {
public:
    explicit SkinnedRectangle(pemu::UiMain *main, const std::vector<std::string> &cfgTree);

    bool load();

private:
    std::vector<std::string> mTree;
    pemu::UiMain *pMain;
    pemu::Skin *pSkin;
    c2d::Io *pIo;
    c2d::Texture *pTex = nullptr;
};

#endif //PEMU_SKINNEDRECTANGLE_H

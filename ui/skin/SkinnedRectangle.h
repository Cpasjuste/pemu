//
// Created by cpasjuste on 13/12/2021.
//

#ifndef PEMU_SKINNEDRECTANGLE_H
#define PEMU_SKINNEDRECTANGLE_H

class SkinnedRectangle : public c2d::RectangleShape {
public:
    explicit SkinnedRectangle(c2dui::Skin *skin, const std::vector<std::string> &cfgTree);
};

#endif //PEMU_SKINNEDRECTANGLE_H

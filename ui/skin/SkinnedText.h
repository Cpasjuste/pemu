//
// Created by cpasjuste on 13/12/2021.
//

#ifndef PEMU_SKINNEDTEXT_H
#define PEMU_SKINNEDTEXT_H

class SkinnedText : public c2d::Text {
public:
    explicit SkinnedText(c2dui::Skin *skin, const std::vector<std::string> &cfgTree);
};

#endif //PEMU_SKINNEDTEXT_H

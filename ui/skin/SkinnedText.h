//
// Created by cpasjuste on 13/12/2021.
//

#ifndef PEMU_SKINNEDTEXT_H
#define PEMU_SKINNEDTEXT_H

class SkinnedText : public c2d::Text {
public:
    explicit SkinnedText(c2dui::UiMain *main, const std::vector<std::string> &cfgTree);

    bool load();

private:
    std::vector<std::string> mTree;
    c2dui::UiMain *pMain;
    c2dui::Skin *pSkin;
    c2d::Io *pIo;
};

#endif //PEMU_SKINNEDTEXT_H

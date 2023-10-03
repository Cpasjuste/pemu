//
// Created by cpasjuste on 08/12/18.
//

#ifndef C2DUI_HELP_H
#define C2DUI_HELP_H

namespace pemu {

    class UiHelp : public SkinnedRectangle {

    public:
        explicit UiHelp(UiMain *ui);

    private:
        float addItem(bool left, c2d::Font *font, const std::vector<Skin::Button *> &buttons,
                      const std::string &name, const c2d::Vector2f &pos);
    };
}

#endif //C2DUI_HELP_H

//
// Created by cpasjuste on 16/02/17.
//

#ifndef PFBA_MENU_H
#define PFBA_MENU_H

#include <vector>

namespace c2dui {

    class OptionMenu {

    public:

        OptionMenu(OptionMenu *parent, std::vector<Option> *options, bool isRomCfg = false);

        ~OptionMenu();

        void addChild(const std::string &title, bool firstIndex = false);

        std::string title;
        std::vector<int> option_ids;
        std::vector<OptionMenu *> childs;
        OptionMenu *parent = NULL;

    };
}

#endif //PFBA_MENU_H

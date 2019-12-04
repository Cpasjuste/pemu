//
// Created by cpasjuste on 16/02/17.
//

#include "c2dui_option.h"
#include "c2dui_option_menu.h"

using namespace c2dui;

OptionMenu::OptionMenu(OptionMenu *parent, std::vector<Option> *options, bool isRomCfg) {

    this->parent = parent;

    if (options == nullptr) {
        return;
    }

    OptionMenu *menu = this;

    for (unsigned int i = 0; i < options->size(); i++) {

        Option option = options->at(i);

        if (option.getFlags() & Option::Flags::HIDDEN) {
            continue;
        }

        if (option.getFlags() & Option::Flags::MENU) {
            // 0 == MAIN_OPTIONS DELIMITER
            if (i == 1 && !isRomCfg) {
                menu->title = option.getName();
            } else {
                menu = new OptionMenu(this, nullptr);
                menu->title = option.getName();
                childs.push_back(menu);
            }
        } else {
            //printf("NEW OPTION: %s (%i)\n", option.getName(), option.index);
            menu->option_ids.push_back(option.getId());
        }
    }
}

void OptionMenu::addChild(const std::string &title, bool firstIndex) {

    auto *m = new OptionMenu(nullptr, nullptr);
    m->title = title;
    if (firstIndex) {
        childs.insert(childs.begin(), m);
    } else {
        childs.push_back(m);
    }
}

OptionMenu::~OptionMenu() {

    for (auto &child : childs) {
        delete child;
    }

    childs.clear();
    option_ids.clear();
}

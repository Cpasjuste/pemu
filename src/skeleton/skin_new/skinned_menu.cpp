//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"

using namespace c2d;
using namespace RRLauncher;

void SkinnedMenu::loadAttributes(tinyxml2::XMLNode *node) {
    std::string value = node->Value();
    tinyxml2::XMLElement *element = node->ToElement();

    if (value == "group") {
        rr_debug("%s (%p): %s", m_node->Value(), this, value.c_str());
        // create listbox items widgets
        if (!m_data_name.empty()) {
            for (size_t i = 0; i < (size_t) m_rows; i++) {
                addGroupItem(element);
            }
        } else {
            addGroupItem(element);
        }
    }

    SkinnedListBox::loadAttributes(node);
}

void SkinnedMenu::addGroupItem(tinyxml2::XMLElement *element) {

}

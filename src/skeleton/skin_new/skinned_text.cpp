//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"

using namespace c2d;
using namespace RRLauncher;

SkinnedText::SkinnedText(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent)
        : Text(), SkinnedWidget(skin, node, parent) {
    rr_debug("%s (%p): parent: %s (%p)", node->Value(), this, m_parent->getName().c_str(), m_parent);
    SkinnedText::load();
}

void SkinnedText::loadAttributes(tinyxml2::XMLNode *node) {
    SkinnedWidget::loadAttributes(node);

    tinyxml2::XMLElement *element = node->ToElement();
    std::string value = element->Value();

    if (value == "string") {
        m_var_str = VariableString(XmlHelper::getXmlText(element));
    } else if (value == "options") {
        std::string style = XmlHelper::getXmlAttribute(element, "style");
        if (!style.empty()) {
            if (style == "bold") {
                setStyle(Text::Style::Bold);
            } else if (style == "italic") {
                setStyle(Text::Style::Italic);
            } else if (style == "underline") {
                setStyle(Text::Style::Underlined);
            } else if (style == "strike") {
                setStyle(Text::Style::StrikeThrough);
            } else {
                setStyle(Text::Style::Regular);
            }
        }
        std::string overflow = XmlHelper::getXmlAttribute(element, "overflow");
        if (!overflow.empty()) {
            if (overflow == "newline") {
                setOverflow(Text::Overflow::NewLine);
                // handle font size in overflow mode
                std::string s = XmlHelper::getXmlAttribute(element, "size");
                int size = (int) XmlHelper::parseSize(s, m_parent->getSize().y);
                if (size > 0) {
                    setCharacterSize(size);
                }
            } else {
                setOverflow(Text::Overflow::Clamp);
            }
        }
        std::string spacing = XmlHelper::getXmlAttribute(element, "spacing");
        if (!spacing.empty()) {
            float f = XmlHelper::parseSize(spacing, m_parent->getSize().y);
            if (f > 0) {
                setLineSpacingModifier((int) f);
            }
        }
    } else if (value == "font") {
        rr_debug("%s (%p): %s", m_node->Value(), this, value.c_str());
        c2d::Font *font = m_skin->getFont(element);
        if (font && font != getFont()) {
            setFont(font);
        }
    }
}

void SkinnedText::onUpdate() {
    if (!isVisible()) {
        return;
    }

    setString(m_var_str.toString(m_skin, m_data_name, m_data_index));
    Text::onUpdate();
}

bool SkinnedText::onInput(c2d::Input::Player *players) {
    if (SkinnedWidget::onButtonEvent(players[0].buttons)) {
        return true;
    }
    return C2DObject::onInput(players);
}

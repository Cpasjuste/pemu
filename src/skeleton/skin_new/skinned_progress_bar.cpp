//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"
#include "skinned_progress_bar.h"

using namespace c2d;
using namespace RRLauncher;

SkinnedProgressBar::SkinnedProgressBar(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent, bool load)
        : SkinnedRectangle(skin, node, parent, load) {
    rr_debug("%s (%p): parent: %s (%p)", node->Value(), this, m_parent->getName().c_str(), m_parent);
    if (load) SkinnedProgressBar::load();
}

void SkinnedProgressBar::load() {
    rr_debug("%s (%p)", m_node->Value(), this);
    SkinnedWidget::load();
}

void SkinnedProgressBar::loadAttributes(tinyxml2::XMLNode *node) {
    SkinnedRectangle::loadAttributes(node);
    std::string value = node->Value();
    tinyxml2::XMLElement *element = node->ToElement();

    if (value == "options") {
        rr_debug("%s (%p): %s", m_node->Value(), this, value.c_str());

        // set direction
        std::string dir = XmlHelper::getXmlAttribute(element, "direction");
        if (dir == "left") {
            m_direction = Direction::Left;
        } else if (dir == "right") {
            m_direction = Direction::Right;
        } else if (dir == "up") {
            m_direction = Direction::Up;
        } else if (dir == "down") {
            m_direction = Direction::Down;
        }

        // set values
        m_var_value = XmlHelper::getXmlAttribute(element, "value");
        m_var_value_max = XmlHelper::getXmlAttribute(element, "value_max");
        m_auto_hide = XmlHelper::getXmlAttribute(element, "autohide") == "true";
        m_size_max = SkinnedProgressBar::getSize();
    }
}

void SkinnedProgressBar::onUpdate() {
    if (isVisible() && (getSize().x <= 0 || getSize().y <= 0)) {
        setVisibility(Visibility::Hidden);
        return;
    }

    auto value = (float) m_skin->getVarInt(m_var_value, m_data_name, m_data_index);
    auto value_max = (float) m_skin->getVarInt(m_var_value_max, m_data_name, m_data_index);
    if ((value > 0 && value != value_max) || !m_auto_hide) {
        setVisibility(Visibility::Visible);
        float progress = value / value_max;
        if (m_direction == Direction::Right) {
            setSize(progress * m_size_max.x, m_size_max.y);
        } else if (m_direction == Direction::Left) {
            setSize(m_size_max.x, progress * m_size_max.y);
        } else if (m_direction == Direction::Down) {
            setSize(m_size_max.x, progress * m_size_max.y);
        }
    } else {
        setSize(0, 0);
    }

    SkinnedRectangle::onUpdate();
}

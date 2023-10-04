//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"

using namespace c2d;
using namespace RRLauncher;

SkinnedRectangle::SkinnedRectangle(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent, bool load)
        : C2DRectangle(Vector2f()), SkinnedWidget(skin, node, parent) {
    rr_debug("%s (%p): parent: %s (%p)", node->Value(), this, m_parent->getName().c_str(), m_parent);
    // do not "autoload" children if requested (child classes, listbox item)
    if (load) SkinnedWidget::load();
}

void SkinnedRectangle::loadAttributes(tinyxml2::XMLNode *node) {
    SkinnedWidget::loadAttributes(node);

    std::string value = node->Value();
    tinyxml2::XMLElement *element = node->ToElement();

    if (value == "rect") {
        rr_debug("%s (%p): radius", m_node->Value(), this);
        float radius = XmlHelper::getXmlAttributeFloat(element, "radius");
        if (radius > 0) {
            setCornersRadius(XmlHelper::getXmlAttributeFloat(element, "radius"));
            setCornerPointCount(8);
        }
    }
}

bool SkinnedRectangle::onInput(c2d::Input::Player *players) {
    if (SkinnedWidget::onButtonEvent(players[0].buttons)) {
        return true;
    }
    return C2DObject::onInput(players);
}

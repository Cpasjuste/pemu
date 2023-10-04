//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"

using namespace c2d;
using namespace RRLauncher;

SkinnedSprite::SkinnedSprite(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent)
        : Sprite(), SkinnedWidget(skin, node, parent) {
    rr_debug("%s (%p): parent: %s (%p)", node->Value(), this, m_parent->getName().c_str(), m_parent);
    SkinnedSprite::load();
}

void SkinnedSprite::loadAttributes(tinyxml2::XMLNode *node) {
    SkinnedWidget::loadAttributes(node);
    tinyxml2::XMLElement *element = node->ToElement();
    std::string value = element->Value();

    if (value == "texture") {
        rr_debug("%s (%p): %s", m_node->Value(), this, value.c_str());
        if (!XmlHelper::getXmlAttribute(element, "textures").empty()) {
            c2d::Texture *tex = m_skin->getTexture(element);
            if (tex && tex != getTexture()) {
                float scaling = std::min(
                        (getSize().x * getScale().x) / (float) tex->getTextureSize().x,
                        (getSize().y * getScale().y) / (float) tex->getTextureSize().y);
                setTexture(tex, false);
                setScale(scaling, scaling);
            }
        } else {
            m_var_str = VariableString(XmlHelper::getXmlAttribute(element, "path"));
        }
    }
}

void SkinnedSprite::onUpdate() {
    if (!isVisible()) {
        return;
    }

    if (m_var_str.hasVars()) {
        std::string path = m_var_str.toString(m_skin, m_data_name, m_data_index);
        if (!path.empty() && path != m_texture_path) {
            m_texture_path = path;
            delete (m_texture);
            m_texture = new C2DTexture(path);
            if (m_texture->available) {
                float scaling = std::min(
                        (getSize().x * getScale().x) / (float) m_texture->getTextureSize().x,
                        (getSize().y * getScale().y) / (float) m_texture->getTextureSize().y);
                setTexture(m_texture, false);
                setScale(scaling, scaling);
            } else {
                delete (m_texture);
                m_texture = nullptr;
            }
        }
    }

    C2DObject::onUpdate();
}

bool SkinnedSprite::onInput(c2d::Input::Player *players) {
    if (SkinnedWidget::onButtonEvent(players[0].buttons)) {
        return true;
    }
    return C2DObject::onInput(players);
}

SkinnedSprite::~SkinnedSprite() {
    delete (m_texture);
}

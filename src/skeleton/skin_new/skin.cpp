//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"

using namespace c2d;
using namespace RRLauncher;
using namespace tinyxml2;

Skin::Skin(const c2d::Vector2f &size) : C2DRenderer(size) {
    m_path = Skin::getIo()->getRomFsPath() + "skins/ui_new";

    // load skin xml
    std::string path = m_path + "/skin.xml";
    XMLError e = m_doc.LoadFile(path.c_str());
    if (e != tinyxml2::XML_SUCCESS) {
        rr_error("%s", tinyxml2::XMLDocument::ErrorIDToName(e));
        return;
    }

    // load skin root
    m_root = m_doc.FirstChildElement("skin");
    if (!m_root) {
        rr_error("<skin> node not found");
        return;
    }

    // get skin name
    m_name = XmlHelper::getName(m_root);

    // load colors
    XMLNode *node = m_root->FirstChildElement("colors");
    if (!node) {
        rr_error("<colors> node not found");
        return;
    }
    XMLNode *child = node->FirstChild();
    while (child) {
        Color color = {child->Value(), Skin::getColor(child->ToElement())};
        rr_debug("new color: %s (%i, %i, %i, %i)", color.name.c_str(),
                 color.object.r, color.object.g, color.object.b, color.object.a);
        m_colors.push_back(color);
        child = child->NextSibling();
    }

    // load fonts
    node = m_root->FirstChildElement("fonts");
    if (!node) {
        rr_error("<fonts> node not found");
        return;
    }
    child = node->FirstChild();
    while (child) {
        XMLElement *elem = child->ToElement();
        Font font = {XmlHelper::getXmlAttribute(elem, "name"), Skin::getFont(elem)};
        rr_debug("new font: %s (%s)", font.name.c_str(), font.object->getPath().c_str());
        child = child->NextSibling();
    }

    // load textures
    node = m_root->FirstChildElement("textures");
    if (!node) {
        rr_error("<textures> node not found");
        return;
    }
    child = node->FirstChild();
    while (child) {
        XMLElement *elem = child->ToElement();
        Skin::Texture tex = {XmlHelper::getXmlAttribute(elem, "name"), Skin::getTexture(elem)};
        rr_debug("new texture: %s (%s)", tex.name.c_str(), tex.object->m_path.c_str());
        child = child->NextSibling();
    }

    loadWidgets();
    //m_doc.SaveFile("test.xml");
}

void Skin::load() {
    // recursive loading
    auto skin = new SkinnedRectangle(this, m_root, this);
    skin->setName("skin");
    Skin::add(skin);
}

void Skin::loadWidgets() {
    std::vector<Widget *> widgets;
    std::vector<XMLNode *> widgetNodes;
    XMLNode *node, *child;

    node = m_root->FirstChildElement("widgets");
    if (!node) {
        rr_error("<widgets> node not found");
        return;
    }

    // load widgets
    child = node->FirstChild();
    while (child) {
        auto widget = new Widget();
        widget->name = XmlHelper::getName(child);
        widget->path = m_path + "/" + XmlHelper::getXmlAttribute(child->ToElement(), "path");

        XMLError err = widget->doc->LoadFile(widget->path.c_str());
        if (err != tinyxml2::XML_SUCCESS) {
            rr_error("%s: %s", widget->name.c_str(), tinyxml2::XMLDocument::ErrorIDToName(err));
            delete (widget);
            continue;
        }

        widget->node = widget->doc->FirstChild();
        if (!widget->node) {
            rr_error("%s: widget file is malformed (%s)", widget->name.c_str(), widget->path.c_str());
            delete (widget);
            continue;
        }

        // keep track of nodes pointing to a widget inside this widget
        std::vector<XMLNode *> nodes = XmlHelper::findNodes(widget->node, "widget");
        widgetNodes.insert(widgetNodes.end(), nodes.begin(), nodes.end());

        // add widget "template" to widget list
        rr_debug("new widget: name: %s, root node: %s, widgets links: %zu",
                 widget->name.c_str(), widget->node->Value(), nodes.size());
        widgets.emplace_back(widget);

        child = child->NextSibling();
    }

    // first, replace "widgets links"
    for (const auto &widgetNode: widgetNodes) {
        for (const auto &widget: widgets) {
            std::string name = XmlHelper::getName(widgetNode);
            if (name == widget->name) {
                // insert widget template
                rr_debug("%s: inserting %s to %s",
                         widget->name.c_str(), widget->node->Value(), widgetNode->Parent()->Value());
                tinyxml2::XMLNode *insertedNode = XmlHelper::copyNode(widget->node, widgetNode->Parent());
                // handle widget override
                XmlHelper::mergeNode(widgetNode, insertedNode);
                // delete temp widget/link node
                widgetNode->GetDocument()->DeleteNode(widgetNode);
                break;
            }
        }
    }

    // now replace main document "widgets links"
    widgetNodes = XmlHelper::findNodes(node->NextSibling(), "widget");
    for (const auto &n: widgetNodes) {
        for (const auto &widget: widgets) {
            std::string name = XmlHelper::getXmlAttribute(n->ToElement(), "name");
            if (name == widget->name) {
                rr_debug("%s: inserting %s to %s",
                         widget->name.c_str(), widget->node->Value(), n->Parent()->Value());
                tinyxml2::XMLNode *insertedNode = XmlHelper::copyNode(widget->node, n->Parent());
                // handle widget override
                XmlHelper::mergeNode(n, insertedNode);
                // delete temp widget/link node
                n->GetDocument()->DeleteNode(n);
                break;
            }
        }
    }

    // cleanup
    for (auto &widget: widgets) {
        delete (widget);
    }
}

c2d::Color Skin::getColor(tinyxml2::XMLElement *element) {
    c2d::Color color{};

    if (element) {
        // try "colors" attribute first
        std::string name = XmlHelper::getXmlAttribute(element, "colors");
        if (!name.empty()) {
            auto it = std::find_if(m_colors.begin(), m_colors.end(), [name](const Color &c) {
                return c.name == name;
            });
            if (it != m_colors.end()) {
                return it->object;
            }
        }

        color.r = XmlHelper::getXmlAttributeInt(element, "r");
        color.g = XmlHelper::getXmlAttributeInt(element, "g");
        color.b = XmlHelper::getXmlAttributeInt(element, "b");
        color.a = XmlHelper::getXmlAttributeInt(element, "a");
    }

    return color;
}

c2d::Font *Skin::getFont(tinyxml2::XMLElement *element) {
    if (element) {
        // try "fonts" attribute first
        std::string name = XmlHelper::getXmlAttribute(element, "fonts");
        if (!name.empty()) {
            auto it = std::find_if(m_fonts.begin(), m_fonts.end(), [name](const Font &f) {
                return f.name == name;
            });
            if (it != m_fonts.end()) {
                return it->object;
            }
        }

        // add new font to font list
        auto font = new c2d::Font();
        std::string filename = XmlHelper::getXmlAttribute(element, "path");
        //font->setFilter(c2d::Texture::Filter::Point); // TODO
        font->loadFromFile(m_path + "/" + filename);
        font->setOffset({0.0, XmlHelper::getXmlAttributeFloat(element, "offset")});
        m_fonts.push_back({XmlHelper::getXmlAttribute(element, "name"), font});
        return font;
    }

    return nullptr;
}

c2d::Texture *Skin::getTexture(tinyxml2::XMLElement *element) {
    if (element) {
        // try "textures" attribute first
        std::string name = XmlHelper::getXmlAttribute(element, "textures");
        if (!name.empty()) {
            auto it = std::find_if(m_textures.begin(), m_textures.end(), [name](const Texture &f) {
                return f.name == name;
            });
            if (it != m_textures.end()) {
                return it->object;
            }
        }

        // add new texture to texture list
        // TODO: use "VariableString"
        std::string path = XmlHelper::getXmlAttribute(element, "path");
        bool has_vars = Utility::contains(path, "${");
        if (has_vars) {
            // split string
            std::vector<std::string> split = c2d::Utility::split(path, "${");
            for (const auto &s: split) {
                size_t start = s.find('}');
                if (start != std::string::npos) {
                    // this is a var...
                    //m_strings.push_back({s.substr(0, start), "", true});
                    if (start + 1 != s.length()) {
                        std::string t = s.substr(start + 1, s.length() - 1);
                        //  m_strings.push_back({t, t, false});
                    }
                } else {
                    //m_strings.push_back({s, s, false});
                }
            }
        }

        auto texture = new C2DTexture(m_path + "/" + path);
        m_textures.push_back({XmlHelper::getXmlAttribute(element, "name"), texture});
        return texture;
    }

    return nullptr;
}

Skin::~Skin() {
    printf("~Skin(%p)\n", this);

    // delete fonts
    for (const auto &font: m_fonts) {
        delete (font.object);
    }

    // delete textures
    for (const auto &texture: m_textures) {
        delete (texture.object);
    }

    m_doc.Clear();
}

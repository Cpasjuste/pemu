//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"

using namespace c2d;
using namespace RRLauncher;
using namespace tinyxml2;

c2d::FloatRect XmlHelper::getRectangle(tinyxml2::XMLElement *element, c2d::C2DObject *object,
                                       const c2d::Vector2f &parentSize) {
    FloatRect rect{};
    float x, y, w, h;

    if (element) {
        x = parseSize(XmlHelper::getXmlAttribute(element, "x"), parentSize.x);
        rect.left = x > 0 ? x : object->getPosition().x;
        y = parseSize(XmlHelper::getXmlAttribute(element, "y"), parentSize.y);
        rect.top = y > 0 ? y : object->getPosition().y;
        w = parseSize(XmlHelper::getXmlAttribute(element, "w"), parentSize.x);
        rect.width = w > 0 ? w : object->getSize().x;
        h = parseSize(XmlHelper::getXmlAttribute(element, "h"), parentSize.y);
        rect.height = h > 0 ? h : object->getSize().y;
    }

    return rect;
}

c2d::Vector2f XmlHelper::getPosition(tinyxml2::XMLElement *element, const Vector2f &parentSize) {
    Vector2f vec{};

    if (element) {
        vec.x = parseSize(XmlHelper::getXmlAttribute(element, "x"), parentSize.x);
        vec.y = parseSize(XmlHelper::getXmlAttribute(element, "y"), parentSize.y);
    }

    return vec;
}

c2d::Vector2f XmlHelper::getSize(tinyxml2::XMLElement *element, const Vector2f &parentSize) {
    Vector2f vec{};

    if (element) {
        vec.x = parseSize(XmlHelper::getXmlAttribute(element, "w"), parentSize.x);
        vec.y = parseSize(XmlHelper::getXmlAttribute(element, "h"), parentSize.y);
    }

    return vec;
}

float XmlHelper::getOutlineSize(tinyxml2::XMLElement *element) {
    float size = 0;

    if (element) {
        size = XmlHelper::getXmlAttributeFloat(element, "size");
    }

    return size;
}

c2d::Visibility XmlHelper::getVisibility(tinyxml2::XMLElement *element) {
    Visibility visibility = Visibility::Visible;
    if (!element) {
        return visibility;
    }

    std::string str = XmlHelper::getXmlText(element);
    if (str == "hidden") {
        visibility = Visibility::Hidden;
    }

    return visibility;
}

std::string XmlHelper::getName(tinyxml2::XMLNode *node) {
    return XmlHelper::getXmlAttribute(node->ToElement(), "name");
}

std::string XmlHelper::getName(tinyxml2::XMLElement *element) {
    return XmlHelper::getXmlAttribute(element, "name");
}

c2d::Origin XmlHelper::getOrigin(tinyxml2::XMLElement *element) {
    Origin origin = Origin::TopLeft;
    if (!element) {
        return origin;
    }

    std::string str = XmlHelper::getXmlText(element);
    if (str == "left") {
        origin = Origin::Left;
    } else if (str == "top_left") {
        origin = Origin::TopLeft;
    } else if (str == "top") {
        origin = Origin::Top;
    } else if (str == "top_right") {
        origin = Origin::TopRight;
    } else if (str == "right") {
        origin = Origin::Right;
    } else if (str == "bottom_right") {
        origin = Origin::BottomRight;
    } else if (str == "bottom") {
        origin = Origin::Bottom;
    } else if (str == "bottom_left") {
        origin = Origin::BottomLeft;
    } else if (str == "center") {
        origin = Origin::Center;
    }

    return origin;
}

//
// tweening
//
c2d::TweenPosition *XmlHelper::getTweenPosition(tinyxml2::XMLElement *element, const c2d::Vector2f &parentSize) {
    if (!element) {
        return nullptr;
    }

    Vector2f from = {
            parseSize(XmlHelper::getXmlAttribute(element, "from.x"), parentSize.x),
            parseSize(XmlHelper::getXmlAttribute(element, "from.y"), parentSize.y)
    };
    Vector2f to = {
            parseSize(XmlHelper::getXmlAttribute(element, "to.x"), parentSize.x),
            parseSize(XmlHelper::getXmlAttribute(element, "to.y"), parentSize.y)
    };
    float duration = XmlHelper::getXmlAttributeFloat(element, "duration");
    TweenLoop loop = TweenLoop::None;
    std::string str = XmlHelper::getXmlAttribute(element, "loop");
    if (str == "loop") {
        loop = TweenLoop::Loop;
    } else if (str == "pingpong") {
        loop = TweenLoop::PingPong;
    }

    return new TweenPosition(from, to, duration, loop, TweenState::Playing);
}

c2d::TweenRotation *XmlHelper::getTweenRotation(tinyxml2::XMLElement *element) {
    if (!element) {
        return nullptr;
    }

    float from = (float) XmlHelper::getXmlAttributeInt(element, "from");
    float to = (float) XmlHelper::getXmlAttributeInt(element, "to");
    float duration = XmlHelper::getXmlAttributeFloat(element, "duration");
    TweenLoop loop = TweenLoop::None;
    std::string str = XmlHelper::getXmlAttribute(element, "loop");
    if (str == "loop") {
        loop = TweenLoop::Loop;
    } else if (str == "pingpong") {
        loop = TweenLoop::PingPong;
    }

    auto tween = new TweenRotation(from, to, duration, loop, TweenState::Playing);
    return tween;
}

c2d::TweenScale *XmlHelper::getTweenScale(tinyxml2::XMLElement *element) {
    if (!element) {
        return nullptr;
    }

    Vector2f from = {
            XmlHelper::getXmlAttributeFloat(element, "from.x"),
            XmlHelper::getXmlAttributeFloat(element, "from.y")
    };
    Vector2f to = {
            XmlHelper::getXmlAttributeFloat(element, "to.x"),
            XmlHelper::getXmlAttributeFloat(element, "to.y")
    };

    float duration = XmlHelper::getXmlAttributeFloat(element, "duration");
    TweenLoop loop = TweenLoop::None;
    std::string str = XmlHelper::getXmlAttribute(element, "loop");
    if (str == "loop") {
        loop = TweenLoop::Loop;
    } else if (str == "pingpong") {
        loop = TweenLoop::PingPong;
    }

    auto tween = new TweenScale(from, to, duration, loop, TweenState::Playing);
    return tween;
}

c2d::TweenColor *XmlHelper::getTweenColor(tinyxml2::XMLElement *element) {
    if (!element) {
        return nullptr;
    }

    c2d::Color from = {XmlHelper::getXmlAttributeInt(element, "fromR"),
                       XmlHelper::getXmlAttributeInt(element, "fromG"),
                       XmlHelper::getXmlAttributeInt(element, "fromB"),
                       XmlHelper::getXmlAttributeInt(element, "fromA")};
    c2d::Color to = {XmlHelper::getXmlAttributeInt(element, "toR"),
                     XmlHelper::getXmlAttributeInt(element, "toG"),
                     XmlHelper::getXmlAttributeInt(element, "toB"),
                     XmlHelper::getXmlAttributeInt(element, "toA")};
    float duration = XmlHelper::getXmlAttributeFloat(element, "duration");
    TweenLoop loop = TweenLoop::None;
    std::string str = XmlHelper::getXmlAttribute(element, "loop");
    if (str == "loop") {
        loop = TweenLoop::Loop;
    } else if (str == "pingpong") {
        loop = TweenLoop::PingPong;
    }

    auto tween = new TweenColor(from, to, duration, loop, TweenState::Playing);
    return tween;
}

c2d::TweenAlpha *XmlHelper::getTweenAlpha(tinyxml2::XMLElement *element) {
    if (!element) {
        return nullptr;
    }

    // parse rectangle outline
    float from = XmlHelper::getXmlAttributeFloat(element, "from");
    float to = XmlHelper::getXmlAttributeFloat(element, "to");
    float duration = XmlHelper::getXmlAttributeFloat(element, "duration");
    TweenLoop loop = TweenLoop::None;
    std::string str = XmlHelper::getXmlAttribute(element, "loop");
    if (str == "loop") {
        loop = TweenLoop::Loop;
    } else if (str == "pingpong") {
        loop = TweenLoop::PingPong;
    }

    auto tween = new TweenAlpha(from, to, duration, loop, TweenState::Playing);
    return tween;
}

// helpers
float XmlHelper::parseSize(const std::string &attribute, float parentSize) {
    if (Utility::contains(attribute, "%")) {
        float scaling = Utility::parseFloat(Utility::remove(attribute, "%")) / 100;
        return parentSize * scaling;
    } else if (Utility::contains(attribute, "-")) {
        return parentSize - Utility::parseFloat(Utility::remove(attribute, "-"));
    }

    return Utility::parseFloat(attribute);
}

std::string XmlHelper::getXmlAttribute(XMLElement *element, const std::string &name) {
    const char *ret;

    if (!element) {
        return "";
    }

    ret = element->Attribute(name.c_str());
    if (!ret) {
        return "";
    }

    return ret;
}

int XmlHelper::getXmlAttributeInt(XMLElement *element, const std::string &name) {
    return Utility::parseInt(getXmlAttribute(element, name));
}

float XmlHelper::getXmlAttributeFloat(XMLElement *element, const std::string &name) {
    return Utility::parseFloat(getXmlAttribute(element, name));
}

std::string XmlHelper::getXmlText(XMLElement *element) {
    if (!element || !element->GetText()) {
        return "";
    }

    return element->GetText();
}

int XmlHelper::getXmlTextInt(XMLElement *element) {
    return Utility::parseInt(getXmlText(element));
}

float XmlHelper::getXmlTextFloat(XMLElement *element) {
    return Utility::parseFloat(getXmlText(element));
}

tinyxml2::XMLNode *XmlHelper::findNode(tinyxml2::XMLNode *node, const std::string &name) {
    while (node) {
        if (getName(node) == name) {
            return node;
        }
        if (node->FirstChildElement()) {
            node = node->FirstChildElement();
        } else if (node->NextSiblingElement()) {
            node = node->NextSiblingElement();
        } else {
            while (node->Parent() && !node->Parent()->NextSiblingElement()) {
                node = node->Parent();
            }
            if (node->Parent() && node->Parent()->NextSiblingElement()) {
                node = node->Parent()->NextSiblingElement();
            } else {
                break;
            }
        }
    }

    return nullptr;
}

std::vector<tinyxml2::XMLNode *> XmlHelper::findNodes(tinyxml2::XMLNode *node, const std::string &name) {
    std::vector<tinyxml2::XMLNode *> nodes;
    while (node) {
        if (node->Value() && node->Value() == name) {
            nodes.emplace_back(node);
        }
        if (node->FirstChildElement()) {
            node = node->FirstChildElement();
        } else if (node->NextSiblingElement()) {
            node = node->NextSiblingElement();
        } else {
            while (node->Parent() && !node->Parent()->NextSiblingElement()) {
                node = node->Parent();
            }
            if (node->Parent() && node->Parent()->NextSiblingElement()) {
                node = node->Parent()->NextSiblingElement();
            } else {
                break;
            }
        }
    }

    return nodes;
}

tinyxml2::XMLNode *XmlHelper::copyNode(const tinyxml2::XMLNode *src, tinyxml2::XMLNode *dst) {
    tinyxml2::XMLNode *newNode = nullptr;

    if (!src || !dst) {
        return newNode;
    }

    tinyxml2::XMLDocument *doc = dst->GetDocument();
    tinyxml2::XMLNode *copy = src->ShallowClone(doc);
    if (!copy) {
        return newNode;
    }

    newNode = dst->InsertEndChild(copy);

    for (const tinyxml2::XMLNode *node = src->FirstChild(); node != nullptr; node = node->NextSibling()) {
        copyNode(node, copy);
    }

    return newNode;
}

tinyxml2::XMLNode *XmlHelper::mergeNode(const tinyxml2::XMLNode *src, tinyxml2::XMLNode *dst) {
    if (!src || !dst) {
        return nullptr;
    }

    const tinyxml2::XMLNode *srcChild = src->FirstChild();
    while (srcChild) {
        auto dstChild = dst->FirstChildElement(srcChild->Value());
        if (!dstChild) {
            rr_debug("NODE (ADD): %s", srcChild->Value());
            copyNode(srcChild, dst);
        } else {
            auto srcElem = srcChild->ToElement();
            auto dstElem = dstChild->ToElement();
            if (srcElem && srcElem->GetText() && dstElem && dstElem->GetText()) {
                std::string srcText = srcElem->GetText();
                std::string dstText = dstElem->GetText();
                if (dstText != srcText) {
                    rr_debug("TEXT (MERGE): %s=%s => %s=%s",
                             srcChild->Value(), srcText.c_str(), dstChild->Value(), dstText.c_str());
                    dstElem->SetText(srcText.c_str());
                }
            } else {
                for (const tinyxml2::XMLAttribute *srcAttr = srcChild->ToElement()->FirstAttribute(); srcAttr; srcAttr = srcAttr->Next()) {
                    for (const tinyxml2::XMLAttribute *dstAttr = dstChild->ToElement()->FirstAttribute(); dstAttr; dstAttr = dstAttr->Next()) {
                        if (std::string(dstAttr->Name()) == srcAttr->Name()
                            && std::string(dstAttr->Value()) != srcAttr->Value()) {
                            rr_debug("ATTR (MERGE): %s=%s => %s=%s",
                                     srcAttr->Name(), srcAttr->Value(), dstAttr->Name(), dstAttr->Value());
                            dstChild->SetAttribute(dstAttr->Name(), srcAttr->Value());
                        }
                    }
                }
                mergeNode(srcChild, dstChild);
            }
        }
        srcChild = srcChild->NextSibling();
    }

    return dst;
}

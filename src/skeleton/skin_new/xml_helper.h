//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_XML_HELPER_H
#define RR_LAUNCHER_XML_HELPER_H

#include <string>
#include <tinyxml2.h>

namespace RRLauncher {
    class XmlHelper {
    public:
        static float parseSize(const std::string &attribute, float parentSize);

        static std::string getName(tinyxml2::XMLNode *node);

        static std::string getName(tinyxml2::XMLElement *element);

        static c2d::FloatRect getRectangle(tinyxml2::XMLElement *element, c2d::C2DObject *object,
                                           const c2d::Vector2f &parentSize);

        static c2d::Vector2f getPosition(tinyxml2::XMLElement *element, const c2d::Vector2f &parentSize);

        static c2d::Vector2f getSize(tinyxml2::XMLElement *element, const c2d::Vector2f &parentSize);

        static float getOutlineSize(tinyxml2::XMLElement *element);

        static c2d::Origin getOrigin(tinyxml2::XMLElement *element);

        static c2d::Visibility getVisibility(tinyxml2::XMLElement *element);

        static c2d::TweenPosition *getTweenPosition(tinyxml2::XMLElement *element, const c2d::Vector2f &parentSize);

        static c2d::TweenRotation *getTweenRotation(tinyxml2::XMLElement *element);

        static c2d::TweenScale *getTweenScale(tinyxml2::XMLElement *element);

        static c2d::TweenColor *getTweenColor(tinyxml2::XMLElement *element);

        static c2d::TweenAlpha *getTweenAlpha(tinyxml2::XMLElement *element);

        //

        static std::string getXmlAttribute(tinyxml2::XMLElement *element, const std::string &name);

        static int getXmlAttributeInt(tinyxml2::XMLElement *element, const std::string &name);

        static float getXmlAttributeFloat(tinyxml2::XMLElement *element, const std::string &name);

        static std::string getXmlText(tinyxml2::XMLElement *element);

        static int getXmlTextInt(tinyxml2::XMLElement *element);

        static float getXmlTextFloat(tinyxml2::XMLElement *element);

        //

        static tinyxml2::XMLNode *findNode(tinyxml2::XMLNode *node, const std::string &name);

        static std::vector<tinyxml2::XMLNode *> findNodes(tinyxml2::XMLNode *node, const std::string &name);

        static tinyxml2::XMLNode *copyNode(const tinyxml2::XMLNode *src, tinyxml2::XMLNode *dst);

        static tinyxml2::XMLNode *mergeNode(const tinyxml2::XMLNode *src, tinyxml2::XMLNode *dst);
    };
}

#endif //RR_LAUNCHER_XML_HELPER_H

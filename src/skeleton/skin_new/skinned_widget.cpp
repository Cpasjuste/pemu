//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"

using namespace c2d;
using namespace RRLauncher;

SkinnedWidget::SkinnedWidget(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent) {
    m_skin = skin;
    m_node = node;
    m_parent = parent;
    m_data_name = XmlHelper::getXmlAttribute(node->ToElement(), "data");
}

void SkinnedWidget::load() {
    auto object = toObject();

    // widget name
    object->setName(XmlHelper::getName(m_node->ToElement()));
    rr_debug("%s (%p)", m_node->Value(), this);

    // default values
    if (std::string(m_node->Value()) != "sprite") {
        object->setFillColor(Color::Transparent);
        object->setOutlineColor(Color::Transparent);
    }
    object->setOrigin(Origin::TopLeft);

    // clear old events
    m_events.clear();

    // parse
    tinyxml2::XMLNode *node = m_node->FirstChild();
    while (node) {
        std::string value = node->Value();
        if (value == "rectangle") {
            auto w = new SkinnedRectangle(m_skin, node, object);
            object->add(w);
        } else if (value == "sprite") {
            auto w = new SkinnedSprite(m_skin, node, object);
            object->add(w);
        } else if (value == "text") {
            auto w = new SkinnedText(m_skin, node, object);
            object->add(w);
        } else if (value == "listbox" || value == "listbox_options") {
            auto w = new SkinnedListBox(m_skin, node, object);
            object->add(w);
        } else if (value == "progressbar") {
            auto w = new SkinnedProgressBar(m_skin, node, object);
            object->add(w);
        } else if (Utility::startWith(value, "tween_")) {
            addTween(node, object);
        } else if (value == "event") {
            loadEvents(node);
        } else {
            loadAttributes(node);
        }
        node = node->NextSibling();
    }
}

void SkinnedWidget::loadAttributes(tinyxml2::XMLNode *node) {
    std::string value = node->Value();
    tinyxml2::XMLElement *element = node->ToElement();
    auto object = toObject();

    if (value == "rect") {
        // parse size and origin
        FloatRect rect = XmlHelper::getRectangle(element, object, m_parent->getSize());
        object->setPosition(rect.left, rect.top);
        object->setSize(rect.width, rect.height);
        rr_debug("%s (%p): %s", object->getName().c_str(), this, value.c_str());
    } else if (value == "position") {
        Vector2f pos = XmlHelper::getPosition(element, m_parent->getSize());
        object->setPosition(pos);
        rr_debug("%s (%p): %s", object->getName().c_str(), this, value.c_str());
    } else if (value == "size") {
        Vector2f size = XmlHelper::getSize(element, m_parent->getSize());
        object->setSize(size);
        rr_debug("%s (%p): %s", object->getName().c_str(), this, value.c_str());
    } else if (value == "color") {
        object->setFillColor(m_skin->getColor(element));
        rr_debug("%s (%p): %s", object->getName().c_str(), this, value.c_str());
    } else if (value == "outline") {
        object->setOutlineColor(m_skin->getColor(element));
        object->setOutlineThickness((float) XmlHelper::getOutlineSize(element));
        rr_debug("%s (%p): %s", object->getName().c_str(), this, value.c_str());
    } else if (value == "visibility") {
        object->setVisibility(XmlHelper::getVisibility(element), true);
        rr_debug("%s: %s == %s", object->getName().c_str(),
                 value.c_str(), XmlHelper::getXmlText(element).c_str());
    } else if (value == "origin") {
        object->setOrigin(XmlHelper::getOrigin(element));
        rr_debug("%s: %s == %s", object->getName().c_str(),
                 value.c_str(), XmlHelper::getXmlText(element).c_str());
    } else if (value == "process_inputs") {
        auto w = dynamic_cast<SkinnedWidget *>(object);
        w->m_process_inputs = XmlHelper::getXmlText(element) == "true";
        rr_debug("%s: %s == %s", object->getName().c_str(),
                 value.c_str(), XmlHelper::getXmlText(element).c_str());
    } else if (value == "data") {
        auto w = dynamic_cast<SkinnedWidget *>(object);
        w->m_data_name = XmlHelper::getXmlAttribute(element, "name");
        rr_debug("%s: %s == %s", object->getName().c_str(),
                 value.c_str(), w->m_data_name.c_str());
    } else {
        // do not handle attribute
        return;
    }
}

void SkinnedWidget::loadEvents(tinyxml2::XMLNode *node) {
    Event event;

    // set event type
    std::string type = XmlHelper::getXmlAttribute(node->ToElement(), "type");
    if (type == "button_pressed") {
        event.type = Event::Type::ButtonPressed;
    } else if (type == "selection_changed") {
        event.type = Event::Type::SelectionChanged;
    } else {
        return;
    }

    // set event node
    event.node = node;

    // parse targets and buttons
    tinyxml2::XMLNode *child = node->FirstChild();
    while (child) {
        std::string value = child->Value();
        if (value == "condition") {
            event.condition.target = XmlHelper::getXmlAttribute(child->ToElement(), "target");
            std::string cond = XmlHelper::getXmlAttribute(child->ToElement(), "equal");
            if (!cond.empty()) {
                event.condition.type = Condition::Type::Equal;
                event.condition.value = cond;
            } else {
                cond = XmlHelper::getXmlAttribute(child->ToElement(), "not_equal");
                if (!cond.empty()) {
                    event.condition.type = Condition::Type::NotEqual;
                    event.condition.value = cond;
                }
            }
        } else if (value == "target") {
            std::string target = XmlHelper::getXmlAttribute(child->ToElement(), "name");
            if (!target.empty()) {
                event.targets.emplace_back(target);
                event.target_objects.resize(event.targets.size());
            }
        } else if (event.type == Event::Type::ButtonPressed && value == "button") {
            std::string button = XmlHelper::getXmlAttribute(child->ToElement(), "name");
            if (!button.empty()) {
                // parse buttons
                if (button == "a") {
                    event.buttons.emplace_back(Input::Button::A);
                } else if (button == "b") {
                    event.buttons.emplace_back(Input::Button::B);
                } else if (button == "x") {
                    event.buttons.emplace_back(Input::Button::X);
                } else if (button == "y") {
                    event.buttons.emplace_back(Input::Button::Y);
                } else if (button == "left") {
                    event.buttons.emplace_back(Input::Button::Left);
                } else if (button == "right") {
                    event.buttons.emplace_back(Input::Button::Right);
                } else if (button == "up") {
                    event.buttons.emplace_back(Input::Button::Up);
                } else if (button == "down") {
                    event.buttons.emplace_back(Input::Button::Down);
                } else if (button == "select") {
                    event.buttons.emplace_back(Input::Button::Select);
                } else if (button == "start") {
                    event.buttons.emplace_back(Input::Button::Start);
                }
            }
        }
        child = child->NextSibling();
    }

    if (!event.targets.empty()) {
        if ((event.type == Event::Type::ButtonPressed && !event.buttons.empty())
            || event.type == Event::Type::SelectionChanged) {
            m_events.emplace_back(event);
        }
    }
}

bool SkinnedWidget::onButtonEvent(unsigned int buttons) {
    bool handled = false;

    if (!m_process_inputs) {
        return handled;
    }

    for (auto &event: m_events) {
        if (event.type == Event::Type::ButtonPressed) {
            for (const auto &button: event.buttons) {
                if (!(buttons & button)) {
                    continue;
                }
                processEvent(&event);
                handled = true;
            }
        }
    }

    return handled;
}

void SkinnedWidget::processEvent(Event *event) {
    for (size_t i = 0; i < event->targets.size(); i++) {
        if (!event->target_objects.at(i)) {
            event->target_objects.at(i) = m_skin->getChild(event->targets.at(i));
        }
        if (event->target_objects.at(i)) {
            tinyxml2::XMLNode *node = event->node->FirstChild();
            auto w = dynamic_cast<SkinnedWidget *>(event->target_objects.at(i));
            while (node) {
                w->loadAttributes(node);
                node = node->NextSibling();
            }
        } else {
            rr_error("event target not found (%s)", event->targets.at(i).c_str());
        }
    }
}

void SkinnedWidget::addTween(tinyxml2::XMLNode *node, c2d::C2DObject *object) {
    std::string value = node->Value();
    tinyxml2::XMLElement *element = node->ToElement();

    if (value == "tween_position") {
        Vector2f size = m_parent ? m_parent->getSize() : toObject()->getSize();
        auto tween = XmlHelper::getTweenPosition(element, size);
        if (tween) {
            object->add(tween);
        }
    } else if (value == "tween_rotation") {
        auto tween = XmlHelper::getTweenRotation(element);
        if (tween) {
            object->add(tween);
        }
    } else if (value == "tween_scale") {
        auto tween = XmlHelper::getTweenScale(element);
        if (tween) {
            object->add(tween);
        }
    } else if (value == "tween_color") {
        auto tween = XmlHelper::getTweenColor(element);
        if (tween) {
            object->add(tween);
        }
    } else if (value == "tween_alpha") {
        auto tween = XmlHelper::getTweenAlpha(element);
        if (tween) {
            object->add(tween);
        }
    }
}

void SkinnedWidget::setData(const std::string &name, int index) {
    m_data_name = name;
    m_data_index = index;
    auto obj = toObject();
    for (const auto &child: obj->getChilds()) {
        dynamic_cast<SkinnedWidget *>(child)->setData(name, index);
    }
}

void SkinnedWidget::setDataName(const std::string &name) {
    m_data_name = name;
    auto obj = toObject();
    for (const auto &child: obj->getChilds()) {
        dynamic_cast<SkinnedWidget *>(child)->setDataName(name);
    }
}

void SkinnedWidget::setDataIndex(int index) {
    m_data_index = index;
    auto obj = toObject();
    for (const auto &child: obj->getChilds()) {
        dynamic_cast<SkinnedWidget *>(child)->setDataIndex(index);
    }
}

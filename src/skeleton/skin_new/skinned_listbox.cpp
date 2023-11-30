//
// Created by cpasjuste on 16/06/22.
//

#include "skin.h"
#include "xml_helper.h"

using namespace c2d;
using namespace RRLauncher;

SkinnedListBox::SkinnedListBox(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent, bool load)
        : SkinnedRectangle(skin, node, parent, false) {
    rr_debug("%s (%p): parent: %s (%p)", node->Value(), this, m_parent->getName().c_str(), m_parent);

    // are we a standard listbox or an options listbox
    m_is_menu = node->Value() == std::string("listbox_options");

    // get listbox rows count attribute
    m_rows = XmlHelper::getXmlAttributeInt(node->ToElement(), "rows");

    // update data list if needed
    if (!m_data_name.empty()) {
        m_skin->updateData(m_data_name);
        // be sure list data count is up-to-date
        m_data_size = (int) m_skin->getDataCount(m_data_name);
    }

    // load attributes and children's
    if (load) SkinnedListBox::load();

    // finally, update listbox items
    SkinnedListBox::updateItems();
}

void SkinnedListBox::load() {
    rr_debug("%s (%p)", m_node->Value(), this);
    SkinnedWidget::load();
}

void SkinnedListBox::loadAttributes(tinyxml2::XMLNode *node) {
    SkinnedRectangle::loadAttributes(node);
    std::string value = node->Value();
    tinyxml2::XMLElement *element = node->ToElement();

    if (value == "data") {
        std::string filter = XmlHelper::getXmlAttribute(element, "filter");
        rr_debug("%s (%p): %s (filter: %s)",
                 m_node->Value(), this, value.c_str(), filter.c_str());
        // handle filtering
        if (filter != m_filter) {
            m_skin->setDataFilter(m_data_name, filter);
        }
        // update listbox items
        m_skin->updateData(m_data_name);
        // be sure list data count is up-to-date
        m_data_size = (int) m_skin->getDataCount(m_data_name);
        // filtering changed, refresh listbox
        if (filter != m_filter) {
            m_filter = filter;
            m_item_index = 0;
            m_highlight_index = 0;
            updateItems();
        }
    } else if (value == "item") {
        rr_debug("%s (%p): %s", m_node->Value(), this, value.c_str());
        // create listbox items widgets
        if (!m_data_name.empty()) {
            for (size_t i = 0; i < (size_t) m_rows; i++) {
                addItem(element);
            }
        } else {
            addItem(element);
            m_data_size++;
        }
    } else if (value == "highlight") {
        rr_debug("%s (%p): %s", m_node->Value(), this, value.c_str());
        auto w = new SkinnedRectangle(m_skin, element, this);
        // set default highlight size
        float height = getSize().y / (float) m_rows;
        w->setSize(getSize().x, height);
        w->load();
        add(w);
        m_highlight = w;
        m_highlight_pos = w->getPosition();
    }
}

void SkinnedListBox::addItem(tinyxml2::XMLElement *element) {
    auto item = new SkinnedRectangle(m_skin, element, this, false);
    float height = getSize().y / (float) m_rows;
    item->setPosition(0, height * (float) m_items.size());
    item->setSize(getSize().x, height); // TODO: fix this
    item->load();
    item->setSize(getSize().x, height - item->getOutlineThickness()); // TODO: fix this
    item->setData(m_data_name, (int) m_items.size());
    add(item);
    m_items.emplace_back(item);

    // duplicate item with "group"
    if (m_is_menu) {
        auto el = element->Parent()->ToElement()->FirstChildElement("group");
        item = new SkinnedRectangle(m_skin, el, this, false);
        height = getSize().y / (float) m_rows;
        item->setPosition(0, height * (float) m_items_options.size());
        item->setSize(getSize().x, height); // TODO: fix this
        item->load();
        item->setSize(getSize().x, height - item->getOutlineThickness()); // TODO: fix this
        item->setData(m_data_name, (int) m_items_options.size());
        item->setVisibility(Visibility::Hidden);
        add(item);
        m_items_options.emplace_back(item);
    }
}

void SkinnedListBox::updateItems() {
    if (m_items.empty()) {
        return;
    }

    //rr_error("%s: m_data_name: %s, m_data_size: %i", getName().c_str(), m_data_name.c_str(), m_data_size);

    for (int i = 0; i < m_rows; i++) {
        if (m_item_index + i >= m_data_size) {
            if ((int) m_items.size() > i) {
                m_items.at(i)->setVisibility(Visibility::Hidden);
                if (m_is_menu) m_items_options.at(i)->setVisibility(Visibility::Hidden);
                rr_error("%s: m_items[%i]->setVisibility(Hidden)",
                         getName().c_str(), i);
            }
            continue;
        }

        // check if we want a group shape
        auto items = &m_items;
        if (m_is_menu) {
            auto str = m_skin->getVar("VALUE", "main_options", m_item_index + i);
            if (str.empty()) {
                items = &m_items_options;
                m_items.at(i)->setVisibility(Visibility::Hidden);
            } else {
                m_items_options.at(i)->setVisibility(Visibility::Hidden);
            }
        }

        items->at(i)->setVisibility(Visibility::Visible);
        auto widget = dynamic_cast<SkinnedWidget *>(items->at(i));
        widget->setDataIndex(m_item_index + i);
        if (i != m_highlight_index) continue;
        m_skin->setDataIndex(m_data_name, m_item_index + i);
        if (m_highlight) {
            m_highlight->setPosition(m_highlight_pos + items->at(i)->getPosition());
        }

        // handle "selection_changed" event
        for (auto &event: m_events) {
            if (event.type != Event::Type::SelectionChanged) continue;
            // check for condition
            if (!event.condition.target.empty() && event.condition.type != Condition::Type::Unknown) {
                auto target = m_items.at(i)->getChild(event.condition.target);
                if (target) {
                    std::string text = ((c2d::Text *) target)->getString();
                    if ((event.condition.type == Condition::Type::Equal && text == event.condition.value)
                        || (event.condition.type == Condition::Type::NotEqual && text != event.condition.value)) {
                        processEvent(&event);
                    }
                }
            } else {
                processEvent(&event);
            }
        }
    }

    if (m_highlight) {
        if (m_data_size <= 0) {
            m_highlight->setVisibility(Visibility::Hidden);
        } else if (!m_highlight->isVisible()) {
            m_highlight->setVisibility(Visibility::Visible);
        }
    }
}

void SkinnedListBox::prev() {
    int index = m_item_index + m_highlight_index;
    int middle = m_rows / 2;

    // be sure list data count is up-to-date
    if (!m_data_name.empty()) {
        m_data_size = (int) m_skin->getDataCount(m_data_name);
    }

    if (m_highlight_index <= middle && index - middle > 0) {
        m_item_index--;
    } else {
        m_highlight_index--;
    }

    if (m_highlight_index < 0) {
        m_highlight_index = m_data_size < m_rows - 1 ? m_data_size - 1 : m_rows - 1;
        m_item_index = (m_data_size - 1) - m_highlight_index;
    }

    updateItems();
}

void SkinnedListBox::next() {
    int index = m_item_index + m_highlight_index;
    int middle = m_rows / 2;

    // be sure list data count is up-to-date
    if (!m_data_name.empty()) {
        m_data_size = (int) m_skin->getDataCount(m_data_name);
    }

    if (m_highlight_index >= middle && index + middle + 1 < m_data_size) {
        m_item_index++;
    } else {
        m_highlight_index++;
    }

    if (m_highlight_index >= m_rows || m_item_index + m_highlight_index >= m_data_size) {
        m_item_index = 0;
        m_highlight_index = 0;
    }

    updateItems();
}

bool SkinnedListBox::onInput(c2d::Input::Player *players) {
    unsigned int buttons = players[0].buttons;

    if (m_process_inputs) {
        if (buttons & Input::Button::Up) prev();
        if (buttons & Input::Button::Down) next();
    }

    return SkinnedWidget::onButtonEvent(buttons);
}

//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKINNED_LISTBOX_H
#define RR_LAUNCHER_SKINNED_LISTBOX_H

namespace RRLauncher {
    class Skin;

    class SkinnedListBox : public SkinnedRectangle {
    public:
        explicit SkinnedListBox(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent, bool load = true);

    protected:
        void load() override;

        void loadAttributes(tinyxml2::XMLNode *node) override;

        bool onInput(c2d::Input::Player *players) override;

        void addItem(tinyxml2::XMLElement *element);

        void updateItems();

        void prev();

        void next();

        int m_data_size = 0;
        int m_rows = 0;
        int m_item_index = 0;
        int m_highlight_index = 0;
        std::vector<c2d::C2DObject *> m_items;
        std::string m_filter;
        // highlight
        c2d::C2DObject *m_highlight = nullptr;
        c2d::Vector2f m_highlight_pos;
    };
}

#endif //RR_LAUNCHER_SKINNED_LISTBOX_H

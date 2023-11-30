//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKINNED_MENU_H
#define RR_LAUNCHER_SKINNED_MENU_H

namespace RRLauncher {
    class Skin;

    class SkinnedMenu : public SkinnedListBox {
    public:
        explicit SkinnedMenu(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent, bool load = true)
                : SkinnedListBox(skin, node, parent, load) {}

    protected:
        void loadAttributes(tinyxml2::XMLNode *node) override;

        void addGroupItem(tinyxml2::XMLElement *element);

    private:
        std::vector<c2d::C2DObject *> m_items_groups;
    };
}

#endif //RR_LAUNCHER_SKINNED_MENU_H

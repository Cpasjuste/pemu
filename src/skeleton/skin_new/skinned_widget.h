//
// Created by cpasjuste on 16/06/22.
//

#ifndef RR_LAUNCHER_SKINNED_WIDGET_H
#define RR_LAUNCHER_SKINNED_WIDGET_H

namespace RRLauncher {
    class Skin;

    class SkinnedWidget {
    public:
        class Condition {
        public:
            enum class Type {
                Equal,
                NotEqual,
                Unknown
            };
            Type type = Type::Unknown;
            std::string target;
            std::string value;
        };

        class Event {
        public:
            enum class Type {
                ButtonPressed,
                SelectionChanged,
                Unknown
            };

            Type type = Type::Unknown;
            Condition condition;
            std::vector<std::string> targets;
            std::vector<c2d::C2DObject *> target_objects;
            tinyxml2::XMLNode *node;
            std::vector<c2d::Input::Button> buttons = {};
            int value = 0;
        };

        explicit SkinnedWidget(Skin *skin, tinyxml2::XMLNode *node, c2d::C2DObject *parent);

        virtual void load();

        virtual void loadAttributes(tinyxml2::XMLNode *node);

        virtual void loadEvents(tinyxml2::XMLNode *node);

        virtual bool onButtonEvent(unsigned int buttons);

        virtual void addTween(tinyxml2::XMLNode *node, c2d::C2DObject *object);

        virtual void setData(const std::string &name, int index);

        virtual void setDataName(const std::string &name);

        virtual std::string getDataName() { return m_data_name; };

        virtual void setDataIndex(int index);

        virtual int getDataIndex() { return m_data_index; };

        virtual c2d::C2DObject *toObject() {
            return dynamic_cast<c2d::C2DObject *>(this);
        }

    protected:
        virtual void processEvent(Event *event);

        Skin *m_skin;
        tinyxml2::XMLNode *m_node = nullptr;
        c2d::C2DObject *m_parent = nullptr;
        std::vector<Event> m_events;
        bool m_process_inputs = true;

        // data/listbox stuff
        std::string m_data_name;
        int m_data_index = -1;
    };
}

#endif //RR_LAUNCHER_SKINNED_WIDGET_H

//
// Created by cpasjuste on 08/12/2021.
//

#ifndef C2D_APP_H
#define C2D_APP_H

#include "skeleton/skin_new/skin.h"
#include "main.h"

namespace RRLauncher {
    class App : public Skin {

    public:
        explicit App(const c2d::Vector2f &size = c2d::Vector2f(0, 0));

        ~App() override;

        std::string getVar(const std::string &var, const std::string &data = {}, int index = -1) override;

        size_t getVarInt(const std::string &var, const std::string &data = {}, int index = -1) override;

        size_t getDataCount(const std::string &name) override;

        void setDataIndex(const std::string &name, size_t index) override;

        void setDataFilter(const std::string &name, const std::string &filter) override;

        void updateData(const std::string &name) override;

        bool quit = false;

    private:
        bool onInput(c2d::Input::Player *players) override;

        void onUpdate() override;

        PEMUConfig *mConfig = nullptr;
        RomList *mRomList = nullptr;
        size_t mRomListIndex = 0;
        size_t mOptionIndex = 0;
    };
}

#endif //C2D_APP_H

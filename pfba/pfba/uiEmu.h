//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIEMU_H
#define PFBA_UIEMU_H

#include <string>

class PFBAGuiEmu : public c2dui::UIEmu {

public:

    explicit PFBAGuiEmu(c2dui::UIMain *ui);

    int load(c2dui::RomList::Rom *rom) override;

    void stop() override;

    void updateFb() override;

private:

    bool onInput(c2d::Input::Player *players) override;

    void onDraw(c2d::Transform &transform, bool draw = true) override;

    void updateFrame() override;

    void renderFrame(bool draw = true);

    bool audio_sync = false;
    bool bForce50Hz = false;
};

#endif //PFBA_UIEMU_H

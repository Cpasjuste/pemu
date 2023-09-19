//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIEMU_H
#define PFBA_UIEMU_H

#include <string>

class PFBAUiEmu : public c2dui::UiEmu {

public:

    explicit PFBAUiEmu(c2dui::UiMain *ui);

    int load(const ss_api::Game &game) override;

    void stop() override;

private:

#ifdef __PFBA_ARM__
    int getSekCpuCore();
#endif

    bool onInput(c2d::Input::Player *players) override;

    void onUpdate() override;

    bool audio_sync = false;

    int frameskip = 0;

    C2DClock clock;
};

#endif //PFBA_UIEMU_H

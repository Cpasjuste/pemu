//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIEMU_H
#define PFBA_UIEMU_H

#include <string>

class PFBAGuiEmu : public c2dui::UIEmu {

public:

    PFBAGuiEmu(c2dui::UIMain *ui);

    int run(c2dui::RomList::Rom *rom);

    void stop();

    int loop();

    void updateFb();

    void updateFrame();

    void renderFrame(bool draw = true, int drawFps = false, float fps = 0);
};

#endif //PFBA_UIEMU_H

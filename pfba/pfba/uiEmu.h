//
// Created by cpasjuste on 01/06/18.
//

#ifndef PFBA_UIEMU_H
#define PFBA_UIEMU_H

#include <string>

class PFBAGuiEmu : public c2dui::C2DUIGuiEmu {

public:

    PFBAGuiEmu(c2dui::C2DUIGuiMain *ui);

    int run(c2dui::C2DUIRomList::Rom *rom);

    void stop();

    int update();

    void updateFb();

    void updateFrame();

    void renderFrame(bool draw = true, int drawFps = false, float fps = 0);
};

#endif //PFBA_UIEMU_H

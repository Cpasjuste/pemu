//
// Created by cpasjuste on 29/05/18.
//

#ifndef PFBA_UI_H
#define PFBA_UI_H

class PFBAGui : public c2dui::C2DUIGuiMain {

public:

    PFBAGui(c2d::Renderer *renderer, c2d::Io *io, c2d::Input *input,
            c2dui::C2DUIConfig *config, c2dui::C2DUISkin *skin);

    void runRom(c2dui::C2DUIRomList::Rom *rom) override;
};

#endif //PFBA_UI_H

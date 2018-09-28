//
// Created by cpasjuste on 29/05/18.
//

#ifndef PFBA_UI_H
#define PFBA_UI_H

class PFBAGui : public c2dui::UIMain {

public:

    PFBAGui(c2d::Renderer *renderer, c2d::Io *io, c2d::Input *input,
            c2dui::Config *config, c2dui::Skin *skin);

    void runRom(c2dui::RomList::Rom *rom) override;
};

#endif //PFBA_UI_H

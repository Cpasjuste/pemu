//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "ui.h"

#include "burner.h"

using namespace c2d;
using namespace c2dui;

PFBAGui::PFBAGui(Renderer *renderer, Io *io, Input *input,
                 C2DUIConfig *config, C2DUISkin *skin)
        : C2DUIGuiMain(renderer, io, input, config, skin) {

    printf("PFBAGui\n");
}

void PFBAGui::runRom(C2DUIRomList::Rom *rom) {

    if (!rom) {
        return;
    }

    nBurnDrvActive = rom->drv;
    if (nBurnDrvActive >= nBurnDrvCount) {
        printf("PFBAGui::runRom: driver not found\n");
        return;
    }

    C2DUIGuiMain::runRom(rom);
}

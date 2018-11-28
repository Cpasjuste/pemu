//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "ui.h"

#include "burner.h"

using namespace c2d;
using namespace c2dui;

PFBAGui::PFBAGui(Renderer *renderer, Config *config, Skin *skin)
        : UIMain(renderer, config, skin) {

    printf("PFBAGui\n");
}

void PFBAGui::runRom(RomList::Rom *rom) {

    if (!rom) {
        return;
    }

    nBurnDrvActive = rom->drv;
    if (nBurnDrvActive >= nBurnDrvCount) {
        printf("PFBAGui::runRom: driver not found\n");
        return;
    }

    UIMain::runRom(rom);
}

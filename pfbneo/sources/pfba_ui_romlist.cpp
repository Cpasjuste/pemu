//
// Created by cpasjuste on 22/11/16.
//

#include "c2dui.h"
#include "pfba_ui_romlist.h"
#include "tchar.h"
#include "burn.h"

void PFBAUIRomList::filterRomList() {

    UIRomList::filterRomList();

    bool byPath = ui->getConfig()->get(Option::Id::GUI_SHOW_ROM_NAMES)->getValueBool();
    if (byPath) {
        for (unsigned int i = 0; i < nBurnDrvCount; i++) {
            nBurnDrvActive = i;
            std::string fbnZip = std::string(BurnDrvGetTextA(DRV_NAME)) + ".zip";
            std::string fbnName = BurnDrvGetTextA(DRV_FULLNAME);
            char *z_name = nullptr;
            BurnDrvGetZipName(&z_name, 0);
            if (z_name != nullptr) {
                fbnZip = std::string(z_name) + ".zip";
            }
            for (size_t j = 0; j < gameList.games.size(); j++) {
                if (gameList.games.at(j).path == fbnZip) {
                    int count = gameList.games.at(j).names.size();
                    for (int k = 0; k < count; k++) {
                        gameList.games.at(j).names[k].text = fbnName;
                    }
                    break;
                }
            }
        }
    }
}

void PFBAUIRomList::sortRomList() {
    gameList.sortAlpha(false);
}

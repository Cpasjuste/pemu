//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "romlist.h"

PFBARomList::PFBARomList(c2dui::UIMain *ui, const std::string &emuVersion)
        : RomList(ui, emuVersion) {

}

void PFBARomList::build() {

    std::string dataPath = ui->getIo()->getDataPath();

    gameList.append(dataPath + "gamelist_coleco.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_COLECO), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_gamegear.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_GAMEGEAR), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_megadriv.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_MEGADRIV), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_msx.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_MSX), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_pce.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_PCE), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_sg1000.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_SG1000), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_sgx.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_SGX), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_sms.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_SMS), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_tg16.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_TG16), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_zx3.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_ZX3), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    gameList.append(dataPath + "gamelist_nes.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_NES), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    RomList::build();
}

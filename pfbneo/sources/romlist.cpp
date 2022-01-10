//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "romlist.h"

void PFBARomList::build() {

    std::string dataPath = ui->getIo()->getRomFsPath();

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

    gameList.append(dataPath + "gamelist_nes_fds.xml", ui->getConfig()->getRomPaths().at(FBN_PATH_NES_FDS), false);
    setLoadingText("Games: %i / %i", gameList.getAvailableCount(), gameList.games.size());

    RomList::build();

    // remove hidden flags for pfba
    ui->getConfig()->get(Option::Id::GUI_FILTER_CLONES)->setFlags(Option::Flags::BOOLEAN);
    ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->setFlags(Option::Flags::STRING);
    ui->getConfig()->get(Option::Id::GUI_FILTER_ROTATION)->setFlags(Option::Flags::STRING);
    ui->getConfig()->get(Option::Id::GUI_FILTER_RESOLUTION)->setFlags(Option::Flags::STRING);
}

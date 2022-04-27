//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "pgen_romlist.h"

void PGENRomList::build(bool addArcadeSystem) {
    std::string dataPath = ui->getIo()->getDataPath();
    if (!ui->getIo()->exist(dataPath + "gamelist.xml")) {
        dataPath = ui->getIo()->getRomFsPath();
    }

    gameList->append(dataPath + "gamelist_sms.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_CHANNELF), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_gamegear.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_COLECO), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_megacd.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_FDS), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_sg1000.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_GAMEGEAR), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    RomList::build();

    // enable system filtering
    ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->setFlags(Option::Flags::STRING);
}

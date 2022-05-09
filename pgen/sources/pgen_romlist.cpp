//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "pgen_romlist.h"

void PGENRomList::build(bool addArcadeSystem, const ss_api::System &system) {
    std::string dataPath = ui->getIo()->getDataPath();
    if (!ui->getIo()->exist(dataPath + "gamelist.xml")) {
        dataPath = ui->getIo()->getRomFsPath();
    }

    gameList->append(dataPath + "gamelist_sms.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_CHANNELF), false,
                     filters, {2, 0, "Master System"});
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_gamegear.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_COLECO), false,
                     filters, {21, 0, "Game Gear"});
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_megacd.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_FDS), false,
                     {".cue", ".iso", ".chd"}, {20, 0, "Mega-CD"});
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

#if 0
    gameList->append(dataPath + "gamelist_sg1000.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_GAMEGEAR), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());
#endif

    RomList::build(addArcadeSystem, {1, 0, "Megadrive"});

    // enable system filtering
    ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->setFlags(Option::Flags::STRING);
}

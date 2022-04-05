//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "pgen_romlist.h"

void PGENRomList::build() {
    std::string dataPath = ui->getIo()->getDataPath();
    if (!ui->getIo()->exist(dataPath + "gamelist.xml")) {
        dataPath = ui->getIo()->getRomFsPath();
    }

    gameList->append(dataPath + "gamelist_sms.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_CHANNELF), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_gamegear_xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_COLECO), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_megacd.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_FDS), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    gameList->append(dataPath + "gamelist_sg1000.xml",
                     ui->getConfig()->getRomPaths().at(FBN_PATH_GAMEGEAR), false, filters);
    setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());

    RomList::build();

    // remove arcade system filtering
    for (auto sys = gameList->systemList.systems.begin(); sys != gameList->systemList.systems.end(); sys++) {
        if (sys->id == 9999) {
            gameList->systemList.systems.erase(sys);
            break;
        }
    }
    // reset system filtering
    Option *sysOpt = ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM);
    std::string sys = sysOpt->getValueString();
    sysOpt->set({"FILTER_SYSTEM", gameList->systemList.getNames(),
                 0, Option::Id::GUI_FILTER_SYSTEM, Option::Flags::STRING});
    // restore config option
    for (int i = 0; i < sysOpt->size(); i++) {
        if (sysOpt->getValueString() == sys) {
            break;
        }
        sysOpt->next();
    }
}

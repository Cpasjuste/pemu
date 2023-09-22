//
// Created by cpasjuste on 02/04/2020.
//

#include "skeleton/pemu.h"
#include "pgen_romlist.h"

struct GameListInfo {
    ss_api::System system;
    std::string name;
    std::string path;
};

void PGENRomList::build(const ss_api::System &system) {
    std::vector<GameListInfo> gameLists = {
            {{2,   0, "Master System"}, "MASTERSYSTEM", "gamelist_sms.xml"},
            {{21,  0, "Game Gear"},     "GAMEGEAR",     "gamelist_gamegear.xml"},
            {{20,  0, "Mega-CD"},       "MEGACD",       "gamelist_megacd.xml"},
            {{109, 2, "SG-1000"},       "SG1000",       "gamelist_sg1000.xml"}
    };

    for (size_t i = 0; i < gameLists.size(); i++) {
        // look for a "gamelist.xml" file inside rom folder, if none found use embedded (romfs) "gamelist.xml"
        std::string gameListPath = ui->getConfig()->getRomPath(gameLists[i].name) + "gamelist.xml";
        if (!ui->getIo()->exist(gameListPath)) {
            gameListPath = ui->getIo()->getRomFsPath() + gameLists[i].path;
            if (!ui->getIo()->exist(gameListPath)) continue;
        }
        gameList->append(gameListPath, ui->getConfig()->getRomPath(gameLists[i].name),
                         false, filters, gameLists[i].system);
        setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());
        printf("RomList::build: %s, games found: %zu / %zu\n",
               gameListPath.c_str(), gameList->getAvailableCount(), gameList->games.size());
    }

    RomList::build({1, 0, "Megadrive"});

    // enable system filtering
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_SYSTEM)->setFlags(0);
}

//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "pgen_romlist.h"

struct GameListInfo {
    std::string name;
    std::string path;
};

void PGENRomList::build(bool addArcadeSystem, const ss_api::System &system) {
    std::vector<GameListInfo> gameLists = {
            {"SMS", "gamelist_sms.xml"},
            {"GAMEGEAR", "gamelist_gamegear.xml"},
            {"MEGACD", "gamelist_megacd.xml"},
#if 0
            {"SG1000","gamelist_sg1000.xml}"
#endif
    };

    bool showAvailableOnly = ui->getConfig()->get(PEMUConfig::Id::GUI_SHOW_AVAILABLE)->getInteger();
    for (size_t i = 0; i < gameLists.size(); i++) {
        // look for a "gamelist.xml" file inside rom folder, if none found use embedded (romfs) "gamelist.xml"
        std::string gameListPath = ui->getConfig()->getRomPath(gameLists[i].name) + "gamelist.xml";
        if (!ui->getIo()->exist(gameListPath)) {
            gameListPath = ui->getIo()->getRomFsPath() + gameLists[i].path;
            if (!ui->getIo()->exist(gameListPath)) continue;
        }
        gameList->append(gameListPath, ui->getConfig()->getRomPath(gameLists[i].name),
                         false, filters, system, showAvailableOnly);
        setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());
        printf("RomList::build: %s, games found: %zu / %zu\n",
               gameListPath.c_str(), gameList->getAvailableCount(), gameList->games.size());
    }

    RomList::build(addArcadeSystem, {1, 0, "Megadrive"});

    // enable system filtering
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_SYSTEM)->setFlags(0);
}

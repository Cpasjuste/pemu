//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "pgen_romlist.h"

void PGENRomList::build(bool addArcadeSystem, const ss_api::System &system) {
    std::vector<std::string> gameLists = {
            "gamelist_sms.xml",
            "gamelist_gamegear.xml",
            "gamelist_megacd.xml",
#if 0
            "gamelist_sg1000.xml
#endif
    };

    for (int i = 0; i < gameLists.size(); i++) {
        // look for a "gamelist.xml" file inside rom folder, if none found use embedded (romfs) "gamelist.xml"
        std::string gameListPath = ui->getConfig()->getRomPaths().at(i + 1) + "gamelist.xml";
        if (!ui->getIo()->exist(gameListPath)) {
            gameListPath = ui->getIo()->getRomFsPath() + gameLists.at(i);
            if (!ui->getIo()->exist(gameListPath)) continue;
        }
        gameList->append(gameListPath, ui->getConfig()->getRomPaths().at(i + 1), false);
        setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());
        printf("RomList::build: %s, games found: %zu / %zu\n",
               gameListPath.c_str(), gameList->getAvailableCount(), gameList->games.size());
    }

    RomList::build(addArcadeSystem, {1, 0, "Megadrive"});

    // enable system filtering
    ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->setFlags(Option::Flags::STRING);
}

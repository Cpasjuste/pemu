//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "romlist.h"

void PFBARomList::build(bool addArcadeSystem, const ss_api::System &system) {
#ifndef __PFBN_LIGHT__
    std::vector<std::string> gameLists = {
            "gamelist_channelf.xml",
            "gamelist_coleco.xml",
            "gamelist_fds.xml",
            "gamelist_gamegear.xml",
            "gamelist_megadrive.xml",
            "gamelist_msx.xml",
            "gamelist_nes.xml",
            "gamelist_ngp.xml",
            "gamelist_pce.xml",
            "gamelist_sg1000.xml",
            "gamelist_sgx.xml",
            "gamelist_sms.xml",
            "gamelist_spectrum.xml",
            "gamelist_tg16.xml"
    };

    bool showAvailableOnly = ui->getConfig()->get(Option::Id::GUI_SHOW_AVAILABLE)->getValueBool();

    for (size_t i = 0; i < gameLists.size(); i++) {
        // look for a "gamelist.xml" file inside rom folder, if none found use embedded (romfs) "gamelist.xml"
        std::string gameListPath = ui->getConfig()->getRomPaths().at(i + 1) + "gamelist.xml";
        if (!ui->getIo()->exist(gameListPath)) {
            gameListPath = ui->getIo()->getRomFsPath() + gameLists.at(i);
            if (!ui->getIo()->exist(gameListPath)) continue;
        }
        gameList->append(gameListPath, ui->getConfig()->getRomPaths().at(i + 1),
                         false, filters, system, showAvailableOnly);
        setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());
        printf("RomList::build: %s, games found: %zu / %zu\n",
               gameListPath.c_str(), gameList->getAvailableCount(), gameList->games.size());
    }
#endif

    RomList::build(addArcadeSystem);

    // remove hidden flags for pfbn
    ui->getConfig()->get(Option::Id::GUI_FILTER_CLONES)->setFlags(Option::Flags::BOOLEAN);
    ui->getConfig()->get(Option::Id::GUI_FILTER_SYSTEM)->setFlags(Option::Flags::STRING);
    ui->getConfig()->get(Option::Id::GUI_FILTER_ROTATION)->setFlags(Option::Flags::STRING);
    ui->getConfig()->get(Option::Id::GUI_FILTER_RESOLUTION)->setFlags(Option::Flags::STRING);
}

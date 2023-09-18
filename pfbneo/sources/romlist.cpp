//
// Created by cpasjuste on 02/04/2020.
//

#include "c2dui.h"
#include "romlist.h"

struct GameListInfo {
    std::string name;
    std::string path;
};

void PFBARomList::build(bool addArcadeSystem, const ss_api::System &system) {
#ifndef __PFBN_LIGHT__
    std::vector<GameListInfo> gameLists = {
            {"CHANNELF",  "gamelist_channelf.xml"},
            {"COLECO",    "gamelist_coleco.xml"},
            {"FDS",       "gamelist_fds.xml"},
            {"GAMEGEAR",  "gamelist_gamegear.xml"},
            {"MEGADRIVE", "gamelist_megadrive.xml"},
            {"MSX",       "gamelist_msx.xml"},
            {"NES",       "gamelist_nes.xml"},
            {"NGP",       "gamelist_ngp.xml"},
            {"PCE",       "gamelist_pce.xml"},
            {"SG1000",    "gamelist_sg1000.xml"},
            {"SGX",       "gamelist_sgx.xml"},
            {"SMS",       "gamelist_sms.xml"},
            {"SPECTRUM",  "gamelist_spectrum.xml"},
            {"TG16",      "gamelist_tg16.xml"}
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
#endif

    RomList::build(addArcadeSystem);

    // remove hidden flags for pfbneo
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_CLONES)->setFlags(0);
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_SYSTEM)->setFlags(0);
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_ROTATION)->setFlags(0);
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_RESOLUTION)->setFlags(0);
}

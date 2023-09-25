//
// Created by cpasjuste on 02/04/2020.
//

#include "skeleton/pemu.h"
#include "pfbneo_romlist.h"

#error "TODO: update for latest changes"

struct GameListInfo {
    ss_api::System system;
    std::string name;
    std::string path;
};

void PFBARomList::build(const ss_api::System &system) {
#ifndef __PFBN_LIGHT__
    std::vector<GameListInfo> gameLists = {
            {{80,  0,  "Channel F"},                   "CHANNELF",  "gamelist_channelf.xml"},
            {{48,  0,  "Colecovision"},                "COLECO",    "gamelist_coleco.xml"},
            {{106, 3,  "Family Computer Disk System"}, "FDS",       "gamelist_fds.xml"},
            {{21,  0,  "Game Gear"},                   "GAMEGEAR",  "gamelist_gamegear.xml"},
            {{1,   0,  "Megadrive"},                   "MEGADRIVE", "gamelist_megadrive.xml"},
            {{113, 0,  "MSX"},                         "MSX",       "gamelist_msx.xml"},
            {{3,   0,  "NES"},                         "NES",       "gamelist_nes.xml"},
            {{82,  0,  "Neo-Geo Pocket Color"},        "NGP",       "gamelist_ngp.xml"},
            {{31,  0,  "PC Engine"},                   "PCE",       "gamelist_pce.xml"},
            {{109, 2,  "SG-1000"},                     "SG1000",    "gamelist_sg1000.xml"},
            {{105, 31, "PC Engine SuperGrafx"},        "SGX",       "gamelist_sgx.xml"},
            {{2,   0,  "Master System"},               "SMS",       "gamelist_sms.xml"},
            {{76,  0,  "ZX Spectrum"},                 "SPECTRUM",  "gamelist_spectrum.xml"},
            {{758, 31, "PC Engine TurboGrafx"},        "TG16",      "gamelist_tg16.xml"}
    };

    //bool showAvailableOnly = ui->getConfig()->get(PEMUConfig::Id::GUI_SHOW_AVAILABLE)->getInteger();
    for (size_t i = 0; i < gameLists.size(); i++) {
        // look for a "gamelist.xml" file inside rom folder, if none found use embedded (romfs) "gamelist.xml"
        std::string gameListPath = ui->getConfig()->getRomPath(gameLists[i].name) + "gamelist.xml";
        if (!ui->getIo()->exist(gameListPath)) {
            gameListPath = ui->getIo()->getRomFsPath() + gameLists[i].path;
            if (!ui->getIo()->exist(gameListPath)) continue;
        }
        gameList->append(gameListPath, ui->getConfig()->getRomPath(gameLists[i].name),
                         false, filters, system);
        setLoadingText("Games: %li / %li", gameList->getAvailableCount(), gameList->games.size());
        printf("RomList::build: %s, games found: %zu / %zu\n",
               gameListPath.c_str(), gameList->getAvailableCount(), gameList->games.size());
    }
#endif

    RomList::build({75, 0, "Mame"});

    // remove hidden flags for pfbneo
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_CLONES)->setFlags(0);
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_SYSTEM)->setFlags(0);
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_ROTATION)->setFlags(0);
    ui->getConfig()->get(PEMUConfig::Id::GUI_FILTER_RESOLUTION)->setFlags(0);
}

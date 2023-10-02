//
// Created by cpasjuste on 28/09/23.
//

#include "skeleton/pemu.h"
#include "pfbneo_romlist.h"
#include "pfbneo_utility.h"

using namespace c2d;
using namespace pemu;

void PFBNRomList::build(const ss_api::GameList::GameAddedCb &cb) {
    float time_start = ui->getElapsedTime().asSeconds();

    // build rom list
    RomList::build([this](Game *game) {
        auto cfgSys = game->system;
        PFBNEOUtility::GameInfo gameInfo = PFBNEOUtility::getGameInfo(*game);
#if 0
        printf("game: %s, path: %s, fbneo system: %s (0x%08x), core system: %s (0x%08x)\n",
               game->path.c_str(), game->romsPath.c_str(),
               gameInfo.system.c_str(), gameInfo.systemId, cfgSys.name.c_str(), cfgSys.id);
#endif
        // replace gamelist.xml name with fbneo name
        if (!gameInfo.name.empty()) {
            game->name = gameInfo.name;
        }

        // replace editor/developer
        if ((game->editor.name.empty() || game->editor.name == "UNKNOWN")
            && !game->developer.name.empty() && game->developer.name != "UNKNOWN") {
            game->editor.name = game->developer.name;
            game->editor.id = game->developer.id;
        } else if ((game->developer.name.empty() || game->developer.name == "UNKNOWN")
                   && !game->editor.name.empty() && game->editor.name != "UNKNOWN") {
            game->developer.name = game->editor.name;
            game->developer.id = game->editor.id;
        }
        if (!gameInfo.manufacturer.empty() && gameInfo.manufacturer != "<unknown>") {
            if (game->editor.name.empty() || game->editor.name == "UNKNOWN") {
                game->editor.name = gameInfo.manufacturer;
            }
            if (game->developer.name.empty() || game->developer.name == "UNKNOWN") {
                game->developer.name = gameInfo.manufacturer;
            }
        }

        // replace system for arcade roms
        if (cfgSys.id == HARDWARE_PREFIX_ARCADE // is an arcade rom (located in arcade roms folder)
            && !gameInfo.sysName.empty() // system was found in fbneo database
            && gameInfo.sysId != 0x0a004822 // not "sbubsm.zip" (Sega Megadrive)
            && gameInfo.sysId != 0x0a004800 // not "topshoot.zip" (Sega Megadrive)
                ) {
            game->system = {gameInfo.sysId, HARDWARE_PREFIX_ARCADE, gameInfo.sysName};
            // add the system to system list if not added yet
            if (!gameList->systemList.find(game->system.id)) {
                gameList->systemList.systems.emplace_back(game->system);
            }
        }

        // replace parent/clone
        if (!gameInfo.parent.empty()) {
            game->cloneOf = gameInfo.parent + ".zip";
        }
    });

    // remove custom "ARCADE" system added from "RomList::build"
    gameList->systemList.remove(HARDWARE_PREFIX_ARCADE);
    // sort systems
    std::sort(gameList->systemList.systems.begin(), gameList->systemList.systems.end(), Api::sortSystemByName);
    // update system filtering option
    auto opt = ui->getConfig()->get(PEMUConfig::OptId::UI_FILTER_SYSTEM);
    opt->setArray(gameList->systemList.getNames());

    // update config
    ui->getConfig()->load();

    float time_spent = ui->getElapsedTime().asSeconds() - time_start;
    printf("PFBNRomList::build(): list built in %f\n", time_spent);
}

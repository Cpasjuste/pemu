//
// Created by cpasjuste on 28/09/23.
//

#include "skeleton/pemu.h"
#include "pfbneo_romlist.h"
#include "pfbneo_utility.h"

using namespace c2d;
using namespace pemu;

void PFBNRomList::build() {
    // build rom list
    RomList::build();

    // clean system list
    gameList->systemList.systems.clear();

    // update games name/system/parent from fbneo drivers database
    PFBNEOUtility::GameInfo gameInfo;
    for (auto &game: gameList->games) {
        auto cfgSystemId = game.system.id;

        // get fbneo information
        gameInfo = PFBNEOUtility::getGameInfo(game);
        // replace gamelist.xml name with fbneo name
        if (!gameInfo.name.empty()) {
            game.name = gameInfo.name;
        }

        // replace system for arcade roms
        if (cfgSystemId == HARDWARE_PREFIX_ARCADE && !gameInfo.system.empty()) {
            game.system = {gameInfo.systemId, 0, gameInfo.system};
        }
        auto sys1 = game.system;
        auto it = std::find_if(gameList->systemList.systems.begin(), gameList->systemList.systems.end(),
                               [sys1](const System &sys2) {
                                   return sys1.id == sys2.id || sys1.name == sys2.name;
                               });
        if (it == gameList->systemList.systems.end()) {
            gameList->systemList.systems.emplace_back(sys1);
        }

        // replace parent/clone
        if (!gameInfo.parent.empty()) {
            game.cloneOf = gameInfo.parent + ".zip";
        }
    }

    std::sort(gameList->systemList.systems.begin(), gameList->systemList.systems.end(), Api::sortSystemByName);
}

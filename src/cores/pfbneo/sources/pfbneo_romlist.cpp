//
// Created by cpasjuste on 28/09/23.
//

#include "skeleton/pemu.h"
#include "pfbneo_romlist.h"
#include "burner.h"

using namespace c2d;
using namespace pemu;

void PFBNRomList::build(const System &system) {
    RomList::build(system);

    // clean system list
    gameList->systemList.systems.clear();

    // update games name/system/parent from fbneo drivers database
    for (auto &game: gameList->games) {
        FBNEOGameInfo gameInfo = getGameInfo(Utility::removeExt(game.path));
        if (!gameInfo.name.empty()) {
            game.name = gameInfo.name;
        }
        if (!gameInfo.system.empty()) {
            game.system = {gameInfo.systemId, 0, gameInfo.system};
            auto sys1 = game.system;
            auto it = std::find_if(gameList->systemList.systems.begin(), gameList->systemList.systems.end(),
                                   [sys1](const System &sys2) {
                                       return sys1.id == sys2.id || sys1.name == sys2.name;
                                   });
            if (it == gameList->systemList.systems.end()) {
                gameList->systemList.systems.emplace_back(sys1);
            }
        }
        if (!gameInfo.parent.empty()) {
            game.cloneOf = gameInfo.parent + ".zip";
        }
    }

    std::sort(gameList->systemList.systems.begin(), gameList->systemList.systems.end(), Api::sortSystemByName);
}

PFBNRomList::FBNEOGameInfo PFBNRomList::getGameInfo(const std::string &driver) {
    FBNEOGameInfo gameInfo;
    for (unsigned int i = 0; i < nBurnDrvCount; i++) {
        nBurnDrvActive = i;
        if (driver == BurnDrvGetTextA(DRV_NAME)) {
            gameInfo.name = BurnDrvGetTextA(DRV_FULLNAME) ? BurnDrvGetTextA(DRV_FULLNAME) : "";
            gameInfo.parent = BurnDrvGetTextA(DRV_PARENT) ? BurnDrvGetTextA(DRV_PARENT) : "";
            //
            unsigned int id = BurnDrvGetHardwareCode();
            unsigned int prefix = ((id | HARDWARE_PREFIX_CARTRIDGE) ^ HARDWARE_PREFIX_CARTRIDGE) & 0xff000000;
            if (prefix == HARDWARE_PREFIX_SNK) {
                gameInfo.systemId = (int) prefix;
                gameInfo.system = "Neo Geo";
            } else if (prefix == HARDWARE_PREFIX_CPS2) {
                gameInfo.systemId = (int) prefix;
                gameInfo.system = "CPS2";
            } else if (prefix == HARDWARE_PREFIX_CPS3) {
                gameInfo.systemId = (int) prefix;
                gameInfo.system = "CPS3";
            } else if (prefix == HARDWARE_PREFIX_CAPCOM) {
                gameInfo.systemId = (int) prefix;
                gameInfo.system = "CPS1";
            } else {
                gameInfo.system = BurnDrvGetTextA(DRV_SYSTEM) ? BurnDrvGetTextA(DRV_SYSTEM) : "";
                gameInfo.systemId = BurnDrvGetHardwareCode();
            }
            break;
        }
    }

    return gameInfo;
}

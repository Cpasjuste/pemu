//
// Created by cpasjuste on 28/09/23.
//

#include "skeleton/pemu.h"
#include "burner.h"
#include "pfbneo_utility.h"

int PFBNEOUtility::setDriverActive(const Game &game) {
    std::string zipName = c2d::Utility::removeExt(game.path);
    if (game.system.id == HARDWARE_PREFIX_COLECO) {
        zipName = "cv_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_SEGA_GAME_GEAR) {
        zipName = "gg_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_SEGA_MEGADRIVE) {
        zipName = "md_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_MSX) {
        zipName = "msx_" + zipName;
    } else if (game.system.id == HARDWARE_PCENGINE_PCENGINE) {
        zipName = "pce_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_SEGA_SG1000) {
        zipName = "sg1k_" + zipName;
    } else if (game.system.id == HARDWARE_PCENGINE_SGX) {
        zipName = "sgx_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_SEGA_MASTER_SYSTEM) {
        zipName = "sms_" + zipName;
    } else if (game.system.id == HARDWARE_PCENGINE_TG16) {
        zipName = "tg_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_SPECTRUM) {
        zipName = "spec_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_NES) {
        zipName = "nes_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_FDS) {
        zipName = "fds_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_CHANNELF) {
        zipName = "chf_" + zipName;
    } else if (game.system.id == HARDWARE_PREFIX_NGP) {
        zipName = "ngp_" + zipName;
    }

    for (unsigned int i = 0; i < nBurnDrvCount; i++) {
        nBurnDrvActive = i;
        if (zipName == BurnDrvGetTextA(DRV_NAME)) {
            break;
        }
    }

    return (int) nBurnDrvActive;
}

PFBNEOUtility::GameInfo PFBNEOUtility::getGameInfo(const Game &game) {
    auto driver = setDriverActive(game);
    if (driver < nBurnDrvCount) {
        GameInfo gameInfo;
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

        return gameInfo;
    }

    return {};
}

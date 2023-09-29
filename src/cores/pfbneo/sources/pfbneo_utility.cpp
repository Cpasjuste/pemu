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
            gameInfo.sysId = (int) prefix;
            gameInfo.sysName = "Neo Geo";
        } else if (prefix == HARDWARE_PREFIX_CPS2) {
            gameInfo.sysId = (int) prefix;
            gameInfo.sysName = "CPS-2";
        } else if (prefix == HARDWARE_PREFIX_CPS3) {
            gameInfo.sysId = (int) prefix;
            gameInfo.sysName = "CPS-3";
        } else if (prefix == HARDWARE_PREFIX_CAPCOM) {
            gameInfo.sysId = (int) prefix;
            gameInfo.sysName = "CPS-1";
        } else {
            gameInfo.sysId = (int) prefix;
            switch (prefix) {
                case HARDWARE_PREFIX_MISC_PRE90S:
                    gameInfo.sysName = "PRE-90S (MISC)";
                    break;
                case HARDWARE_PREFIX_SEGA:
                    gameInfo.sysName = "SEGA (ARCADE)";
                    break;
                case HARDWARE_PREFIX_KONAMI:
                    gameInfo.sysName = "KONAMI";
                    break;
                case HARDWARE_PREFIX_TOAPLAN:
                    gameInfo.sysName = "TOAPLAN";
                    break;
                case HARDWARE_PREFIX_CAVE:
                    gameInfo.sysName = "CAVE";
                    break;
                case HARDWARE_PREFIX_IGS_PGM:
                    gameInfo.sysName = "IGS PGM";
                    break;
                case HARDWARE_PREFIX_MISC_POST90S:
                    gameInfo.sysName = "POST-90S (MISC)";
                    break;
                case HARDWARE_PREFIX_TAITO:
                    gameInfo.sysName = "TAITO";
                    break;
                case HARDWARE_PREFIX_PSIKYO:
                    gameInfo.sysName = "PSIKYO";
                    break;
                case HARDWARE_PREFIX_KANEKO:
                    gameInfo.sysName = "KANEKO";
                    break;
                case HARDWARE_PREFIX_PACMAN:
                    gameInfo.sysName = "PACMAN";
                    break;
                case HARDWARE_PREFIX_GALAXIAN:
                    gameInfo.sysName = "GALAXIAN";
                    break;
                case HARDWARE_PREFIX_IREM:
                    gameInfo.sysName = "IREM";
                    break;
                case HARDWARE_PREFIX_DATAEAST:
                    gameInfo.sysName = "DATAEAST";
                    break;
                case HARDWARE_PREFIX_CAPCOM_MISC:
                    gameInfo.sysName = "CAPCOM (MISC)";
                    break;
                case HARDWARE_PREFIX_SETA:
                    gameInfo.sysName = "SETA";
                    break;
                case HARDWARE_PREFIX_TECHNOS:
                    gameInfo.sysName = "TECHNOS";
                    break;
                case HARDWARE_PREFIX_MIDWAY:
                    gameInfo.sysName = "MIDWAY";
                    break;
                default:
                    break;
            }
        }

        return gameInfo;
    }

    return {};
}

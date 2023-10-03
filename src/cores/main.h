//
// Created by cpasjuste on 19/09/23.
//

#ifndef PEMU_MAIN_H
#define PEMU_MAIN_H

#include "skeleton/pemu.h"

#ifdef __PS4__
extern "C" int sceSystemServiceLoadExec(const char *path, const char *args[]);
#endif

#ifdef __PFBA__
#include "pfbneo_ui_emu.h"
#include "pfbneo_ui_menu.h"
#include "pfbneo_config.h"
#include "pfbneo_ui_menu_state.h"
#include "pfbneo_io.h"
#include "pfbneo_romlist.h"
#define PEMUIo PFBAIo
#define PEMUConfig PFBAConfig
#define PEMURomList PFBNRomList
#define PEMUSkin pemu::Skin
#define PEMUUiMain pemu::UiMain
#define PEMUUiEmu PFBAUiEmu
#define PEMUUiMenu PFBAGuiMenu
#define PEMUUiMenuState PFBAUIStateMenu
#define PEMUUiRomList UIRomList
#elif __PGEN__
#include "pgen_io.h"
#include "pgen_config.h"
#include "pgen_ui_emu.h"
#include "pgen_ui_menu_state.h"
#define PEMUIo PGENIo
#define PEMUConfig PGENConfig
#define PEMURomList RomList
#define PEMUSkin pemu::Skin
#define PEMUUiMain pemu::UiMain
#define PEMUUiEmu PGENUiEmu
#define PEMUUiMenu pemu::UiMenu
#define PEMUUiMenuState PGENUIStateMenu
#define PEMUUiRomList UIRomList
#elif __PNES__
#include "pnes_ui_emu.h"
#include "pnes_ui_menu_state.h"
#include "pnes_config.h"
#include "pnes_io.h"
#define PEMUIo PNESIo
#define PEMUConfig PNESConfig
#define PEMURomList RomList
#define PEMUSkin Skin
#define PEMUUiMain UiMain
#define PEMUUiEmu PNESUiEmu
#define PEMUUiMenu UiMenu
#define PEMUUiMenuState PNESUIStateMenu
#define PEMUUiRomList UIRomList
#elif __PSNES__
#include "psnes_ui_emu.h"
#include "psnes_ui_menu_state.h"
#include "psnes_config.h"
#include "psnes_io.h"
#define PEMUIo PSNESIo
#define PEMUConfig PSNESConfig
#define PEMURomList RomList
#define PEMUSkin Skin
#define PEMUUiMain UiMain
#define PEMUUiEmu PSNESUiEmu
#define PEMUUiMenu UiMenu
#define PEMUUiMenuState PSNESUIStateMenu
#define PEMUUiRomList UIRomList
#elif __PGBA__
#include "pgba_io.h"
#include "pgba_config.h"
#include "pgba_ui_emu.h"
#include "pgba_ui_menu_state.h"
#define PEMUIo PGBAIo
#define PEMUConfig PGBAConfig
#define PEMURomList pemu::RomList
#define PEMUSkin pemu::Skin
#define PEMUUiMain pemu::UiMain
#define PEMUUiEmu PGBAUiEmu
#define PEMUUiMenu pemu::UiMenu
#define PEMUUiMenuState PGBAUIStateMenu
#define PEMUUiRomList pemu::UIRomList
#endif

#endif //PEMU_MAIN_H

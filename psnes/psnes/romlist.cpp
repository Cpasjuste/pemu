//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "romlist.h"
#include "tinyxml2.h"

using namespace c2d;
using namespace c2dui;
using namespace tinyxml2;

static XMLDocument doc;

static bool sortByName(const C2DUIRomList::Rom *ra, const C2DUIRomList::Rom *rb) {
    return strcasecmp(ra->name, rb->name) <= 0;
}

PSNESRomList::PSNESRomList(C2DUIGuiMain *ui, const std::string &emuVersion) : C2DUIRomList(ui, emuVersion) {
    printf("PSNESRomList::PSNESRomList()\n");
}

void PSNESRomList::buildNoDb(bool use_icons) {

    printf("PSNESRomList::buildNoDb()\n");

    for (auto &fileList : files) {

        if (fileList.empty()) {
            continue;
        }

        for (auto &file : fileList) {
            if (!Utility::endsWith(file, ".zip")
                && !Utility::endsWith(file, ".smc")
                && !Utility::endsWith(file, ".sfc")) {
                continue;
            }
            auto *rom = new Rom();
            rom->name = rom->drv_name = file.c_str();
            rom->path = file.c_str();
            rom->state = RomState::WORKING;
            hardwareList->at(0).supported_count++;
            hardwareList->at(0).available_count++;
            rom->color = COL_GREEN;
            // load icon if needed, only for parent roms
            if (use_icons && !rom->parent) {
                // try removing the extension (drv_name has extension (.zip, .smc) with psnes and no db.xml)
                char *drv_name_no_ext = Utility::removeExt(rom->drv_name, '/');
                if (drv_name_no_ext) {
                    snprintf(icon_path, 1023, "%sicons/%s.png",
                             ui->getConfig()->getHomePath()->c_str(), drv_name_no_ext);
                    if (ui->getIo()->exist(icon_path)) {
                        rom->icon = new C2DTexture(icon_path);
                        rom->icon->setDeleteMode(C2DObject::DeleteMode::Manual);
                        if (!rom->icon->available) {
                            delete (rom->icon);
                            rom->icon = nullptr;
                        }
                    }
                    free(drv_name_no_ext);
                }
            }
            list.push_back(rom);

            // UI
            if (list.size() % 100 == 0) {
                snprintf(text_str, 511, "Scanning... %i / %i",
                         hardwareList->at(0).available_count, (int) list.size());
                text->setString(text_str);
                ui->getRenderer()->flip();
            }
            // UI
        }
    }

    std::sort(list.begin(), list.end(), sortByName);

    // cleanup
    C2DUIRomList::build();
}

void PSNESRomList::build() {

    printf("PSNESRomList::build()\n");

    bool use_icons = ui->getConfig()->getValue(C2DUIOption::Index::GUI_SHOW_ICONS) == 1;

    if (!ui->getConfig()->getValue(C2DUIOption::Index::GUI_USE_DATABASE)) {
        buildNoDb(use_icons);
        return;
    }

    char path[MAX_PATH];
    char pathUppercase[MAX_PATH]; // sometimes on FAT32 short files appear as all uppercase

    char xmlPath[512];
    snprintf(xmlPath, 511, "%sdb.xml", ui->getConfig()->getHomePath()->c_str());
    XMLError e = doc.LoadFile(xmlPath);
    if (e != XML_SUCCESS) {
        printf("error: %s\n", tinyxml2::XMLDocument::ErrorIDToName(e));
        ui->getUiMessageBox()->show("ERROR", "Could not load db.xml\n\nWill just add any found files...");
        buildNoDb(use_icons);
        return;
    }

    // try "http://datomatic.no-intro.org/" format
    XMLNode *pRoot = doc.FirstChildElement("datafile");
    if (!pRoot) {
        // try "http://hyperspin-fe.com/" format
        pRoot = doc.FirstChildElement("menu");
        if (!pRoot) {
            printf("error: incorrect db.xml format\n\nWill just add any found files...");
            ui->getUiMessageBox()->show("ERROR", "incorrect db.xml format");
            buildNoDb(use_icons);
            return;
        }
    }

    XMLNode *pGame = pRoot->FirstChildElement("game");
    if (!pGame) {
        printf("error: <game> node not found, incorrect format\n\nWill just add any found files...");
        ui->getUiMessageBox()->show("ERROR", "incorrect db.xml format");
        buildNoDb(use_icons);
        return;
    }

    while (pGame != nullptr) {

        auto *rom = new Rom();

        // get "name"
        rom->name = rom->drv_name = rom->path = pGame->ToElement()->Attribute("name");
        // get "cloneof"
        XMLElement *element = pGame->FirstChildElement("cloneof");
        if (element != nullptr && element->GetText()) {
            rom->parent = element->GetText();
        } else {
            rom->parent = pGame->ToElement()->Attribute("cloneof");
        }
        // get "year"
        element = pGame->FirstChildElement("year");
        if (element != nullptr && element->GetText()) {
            rom->year = element->GetText();
        }
        // get "manufacturer"
        element = pGame->FirstChildElement("manufacturer");
        if (element && element->GetText()) {
            rom->manufacturer = element->GetText();
        }

        // load icon if needed, only for parent roms
        if (use_icons && !rom->parent) {
            snprintf(icon_path, 1023, "%sicons/%s.png",
                     ui->getConfig()->getHomePath()->c_str(), rom->drv_name);
            if (ui->getIo()->exist(icon_path)) {
                rom->icon = new C2DTexture(icon_path);
                rom->icon->setDeleteMode(C2DObject::DeleteMode::Manual);
                if (!rom->icon->available) {
                    delete (rom->icon);
                    rom->icon = nullptr;
                }
            }
        }

        // add rom to "ALL" game list
        hardwareList->at(0).supported_count++;
        if (rom->parent) {
            hardwareList->at(0).clone_count++;
        }

        snprintf(path, 511, "%s.zip", rom->name);
        for (int k = 0; k < (int) strlen(path); k++) {
            pathUppercase[k] = (char) toupper(path[k]);
        }

        for (auto &fileList : files) {
            if (fileList.empty()) {
                continue;
            }
            auto file = std::find(fileList.begin(), fileList.end(), path);
            if (file == fileList.end()) {
                file = std::find(fileList.begin(), fileList.end(), pathUppercase);
            }
            if (file != fileList.end()) {
                rom->path = file->c_str();
                rom->state = RomState::WORKING;
                hardwareList->at(0).available_count++;
                if (rom->parent) {
                    hardwareList->at(0).available_clone_count++;
                }
                break;
            }
        }

        // set "Io::File"" color for ui
        rom->color = COL_RED;
        if (rom->state == C2DUIRomList::RomState::NOT_WORKING) {
            rom->color = COL_ORANGE;
        } else if (rom->state == C2DUIRomList::RomState::WORKING) {
            rom->color = rom->parent ? COL_YELLOW : COL_GREEN;
        }

        if (rom->state == RomState::MISSING) {
            hardwareList->at(0).missing_count++;
            if (rom->parent) {
                hardwareList->at(0).missing_clone_count++;
            }
        }

        //printf("add rom: %s\n", rom->name);
        list.push_back(rom);
        pGame = pGame->NextSibling();

        // UI
        if (list.size() % 100 == 0) {
            snprintf(text_str, 511, "Scanning... %i / %i",
                     hardwareList->at(0).available_count, (int) list.size());
            text->setString(text_str);
            ui->getRenderer()->flip();
        }
        // UI
    }

    std::sort(list.begin(), list.end(), sortByName);

    if (use_icons) {
        // set childs with no icon to parent icon
        for (auto rom : list) {
            if (!rom->icon && rom->parent) {
                auto it = std::find_if(list.begin(), list.end(), [&](const Rom *r) {
                    return r->drv_name == rom->parent;
                });
                if (it != list.end()) {
                    rom->icon = (*it)->icon;
                }
            }
        }
    }

    // cleanup
    C2DUIRomList::build();
}

PSNESRomList::~PSNESRomList() {

    doc.Clear();
}

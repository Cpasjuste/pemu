//
// Created by cpasjuste on 29/05/18.
//

#include "c2dui.h"
#include "romlist.h"

using namespace c2d;
using namespace c2dui;

static bool sortByName(const RomList::Rom *ra, const RomList::Rom *rb) {
    return strcasecmp(ra->name.c_str(), rb->name.c_str()) <= 0;
}

PNESRomList::PNESRomList(UIMain *ui, const std::string &emuVersion) : RomList(ui, emuVersion) {
    printf("PSNESRomList::PSNESRomList()\n");
}

void PNESRomList::build() {

    printf("PSNESRomList::build()\n");

    bool use_icons = ui->getConfig()->getValue(Option::Index::GUI_SHOW_ICONS) == 1;

    for (auto &fileList : files) {

        if (fileList.empty()) {
            continue;
        }

        for (auto &file : fileList) {
            if (!Utility::endsWith(file.name, ".zip")
                && !Utility::endsWith(file.name, ".nes")) {
                continue;
            }
            auto *rom = new Rom();
            rom->name = file.name;
            rom->drv_name = file.name.c_str();
            rom->path = file.path;
            rom->state = RomState::WORKING;
            hardwareList->at(0).supported_count++;
            hardwareList->at(0).available_count++;
            rom->color = COL_GREEN;
            // load icon if needed, only for parent roms
            if (use_icons && !rom->parent) {
                // try removing the extension (drv_name has extension (.zip, .smc) with psnes and no db.xml)
                std::string name = Utility::removeExt(rom->drv_name);
                snprintf(icon_path, 1023, "%sicons/%s.png",
                         ui->getConfig()->getHomePath()->c_str(), name.c_str());
                if (ui->getIo()->exist(icon_path)) {
                    rom->icon = new C2DTexture(icon_path);
                    rom->icon->setDeleteMode(DeleteMode::Manual);
                    if (!rom->icon->available) {
                        delete (rom->icon);
                        rom->icon = nullptr;
                    }
                }
            }
            list.push_back(rom);
        }
    }

    std::sort(list.begin(), list.end(), sortByName);

    // cleanup
    RomList::build();
}

PNESRomList::~PNESRomList() {
}

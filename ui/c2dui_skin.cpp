//
// Created by cpasjuste on 23/11/16.
//

#include <minizip/unzip.h>

#include "c2dui.h"

using namespace c2d;
using namespace c2dui;

Skin::Skin(UIMain *u, const std::vector<Button> &btns) {

    ui = u;
    path = ui->getIo()->getHomePath() + "skins/";

    // extract config file from zipped skin but create a default one
    config = new config::Config("SKIN_CONFIG", path + "default/config.cfg");

    std::string skinName = ui->getConfig()->get(Option::GUI_SKIN)->getValueString() + std::string(".zip");
    int configLen = 0;
    char *configData = getZippedData(path + skinName, "config.cfg", &configLen);
    if (!configData) {
        ui->getConfig()->get(Option::GUI_SKIN)->setValueString("default");
        skinName = "default.zip";
        configData = getZippedData(path + skinName, "config.cfg", &configLen);
    }
    if (configData) {
        configData[configLen - 1] = '\0';
        path += skinName;
        useZippedSkin = true;
        printf("Skin: zipped skin found: %s\n", path.c_str());
    } else {
        path += "default/";
    }

    // TODO: cleanup this
    // load buttons textures
    buttons = btns;
    for (auto &button : buttons) {
        if (useZippedSkin) {
            std::string buttonPath = "buttons/" + std::to_string(button.id) + ".png";
            int size = 0;
            char *data = getZippedData(path, buttonPath, &size);
            if (data) {
                button.texture = new C2DTexture((const unsigned char *) data, size);
                free(data);
            }
        } else {
            std::string buttonPath =
                    ui->getIo()->getHomePath() + "skins/buttons/" + std::to_string(button.id) + ".png";
            button.texture = new C2DTexture(buttonPath);
        }
        if (button.texture && !button.texture->available) {
            delete (button.texture);
        }
    }

    ///
    /// GENERAL
    ///
    config::Group gen("GENERAL");
    gen.addOption({"global_scaling", Vector2f{1.0f, 1.0f}});
    config->addGroup(gen);

    ///
    /// FONT
    ///
    config::Group font_group("FONT");
    font_group.addOption({"path", "default.ttf"});
    font_group.addOption({"offset", Vector2f{0, -3}});
    font_group.addOption({"filtering", 0});
    config->addGroup(font_group);

    ///
    /// HIGHLIGHT
    ///
    config::Group highlight = createRectangleShapeGroup(
            "HIGHLIGHT", {0, 0, 16, 16},
            Origin::Left, "highlight_bg.png", {100, 255, 255, 80}, {0, 255, 255, 80}, 10);
    config->addGroup(highlight);

    ///
    /// MESSAGEBOX
    ///
    config::Group mbox = createRectangleShapeGroup(
            "MESSAGEBOX", {ui->getSize().x / 2, ui->getSize().y / 2,
                           ui->getSize().x / 2, ui->getSize().y / 2},
            Origin::Center, "messagebox_bg.png", Color::GrayLight, Color::Red, 2);
    config->addGroup(mbox);

    ///
    /// MAIN (ROM LIST)
    ///
    config::Group main = createRectangleShapeGroup(
            "MAIN", ui->getLocalBounds(), Origin::TopLeft,
            "romlist_bg.png", Color::GrayDark, Color::Yellow, 2);
    // rom list title + text
    FloatRect titleRect = {ui->getSize().x / 2, 160, ui->getSize().x / 2, 64};
    config::Group title = createRectangleShapeGroup(
            "TITLE", titleRect, Origin::Center,
            "romlist_title.png", Color::GrayDark, Color::GrayLight, 2);
    FloatRect titleTextRect = {titleRect.width / 2, titleRect.height / 2, titleRect.width - 64, 0};
    config::Group titleText = createTextGroup(
            "TEXT", ui->getFontSize(), titleTextRect, Origin::Center, Color::White, Color::Black, 2);
    title.addGroup(titleText);
    main.addGroup(title);
    config::Group help = createRectangleShapeGroup(
            "HELP", titleRect, Origin::Center,
            "romlist_help.png", Color::GrayDark, Color::GrayLight, 2);
    main.addGroup(help);
    config::Group romList = createRectangleShapeGroup(
            "ROM_LIST", {0, 0, 212, 240}, Origin::Left,
            "romlist.png", {255, 255, 255, 150}, Color::GrayLight, 2);
    config::Group romItem = createTextGroup(
            "TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romItem.addOption({"color_missing", Color::White});
    romItem.addOption({"color_not_working", Color::White});
    romItem.addOption({"highlight_use_text_color", 0});
    romList.addGroup(romItem);
    main.addGroup(romList);
    //
    config::Group romSyno = createRectangleShapeGroup(
            "ROM_SYNOPSIS", {654, 378, 608, 322}, Origin::Left,
            "romlist_synopsys.png", {150, 150, 150, 255}, {220, 0, 0, 255}, 2);
    config::Group romSynoText = createTextGroup(
            "TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romSyno.addGroup(romSynoText);
    main.addGroup(romSyno);
    //
    config::Group romInfo = createRectangleShapeGroup(
            "ROM_INFOS", {654, 378, 608, 322}, Origin::Left,
            "romlist_info.png", {150, 150, 150, 255}, {220, 0, 0, 255}, 2);
    config::Group romInfoText = createTextGroup(
            "SYSTEM_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "DEVELOPER_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "EDITOR_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "DATE_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "GENRE_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "PLAYERS_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "RATING_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "TOPSTAFF_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "ROTATION_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "RESOLUTION_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "CLASSIFICATION_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "CLONEOF_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    romInfoText = createTextGroup(
            "FILENAME_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    romInfo.addGroup(romInfoText);
    main.addGroup(romInfo);
    //
    config::Group previewBox = createRectangleShapeGroup(
            "ROM_IMAGE", {654, 378, 608, 322}, Origin::Left,
            "romlist_image.png", {150, 150, 150, 255}, {220, 0, 0, 255}, 2);
    config::Group previewBoxText = createTextGroup(
            "TEXT", ui->getFontSize(), titleTextRect, Origin::Center, Color::White, Color::Black, 1);
    previewBox.addGroup(previewBoxText);
    main.addGroup(previewBox);
    config->addGroup(main);
    ///
    /// MAIN (ROM LIST) (END)
    ///

    ///
    /// OPTIONS_MENU
    ///
    config::Group options_menu = createRectangleShapeGroup(
            "OPTIONS_MENU", ui->getLocalBounds(), Origin::TopLeft,
            "options_menu_bg.png", Color::GrayDark, Color::Yellow, 2);
    config::Group options_menu_title = createTextGroup(
            "TITLE_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    options_menu.addGroup(options_menu_title);
    config::Group options_menu_text = createTextGroup(
            "ITEMS_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    options_menu.addGroup(options_menu_text);
    config->addGroup(options_menu);
    ///
    /// OPTIONS_MENU (END)
    ///

    ///
    /// STATES_MENU
    ///
    config::Group states_menu = createRectangleShapeGroup(
            "STATES_MENU", ui->getLocalBounds(), Origin::TopLeft,
            "states_menu_bg.png", Color::GrayDark, Color::Yellow, 2);
    config::Group states_menu_title = createTextGroup(
            "TITLE_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    states_menu.addGroup(states_menu_title);
    config::Group states_item = createRectangleShapeGroup(
            "STATES_ITEM", ui->getLocalBounds(), Origin::TopLeft,
            "states_item_bg.png", Color::GrayDark, Color::Yellow, 2);
    config::Group states_item_text = createTextGroup(
            "STATES_TEXT", ui->getFontSize(), titleTextRect, Origin::TopLeft, Color::White, Color::Black, 1);
    states_item.addGroup(states_item_text);
    states_menu.addGroup(states_item);
    config->addGroup(states_menu);
    ///
    /// STATES_MENU (END
    ///

    // LOAD/SAVE
    if (useZippedSkin) {
        config->loadFromString(configData);
        free(configData);
    } else {
        if (!config->load()) {
            // file doesn't exist or is malformed, (re)create it
            config->save();
        }
    }


    ///
    /// load global scaling from loaded configuration
    ///
    c2d::config::Group *genGrp = config->getGroup("GENERAL");
    if (genGrp) {
        c2d::config::Option *opt = genGrp->getOption("global_scaling");
        if (opt) {
            global_scaling = opt->getVector2f();
            global_scaling.x *= (float) ui->getConfig()->get(c2dui::Option::Id::GUI_WINDOW_WIDTH)->getValueInt()
                                / (float) ui->getConfig()->get(c2dui::Option::Id::GUI_SCREEN_WIDTH)->getValueInt();
            global_scaling.y *= (float) ui->getConfig()->get(c2dui::Option::Id::GUI_WINDOW_HEIGHT)->getValueInt()
                                / (float) ui->getConfig()->get(c2dui::Option::Id::GUI_SCREEN_HEIGHT)->getValueInt();
        }
    }

    ///
    /// load skin font from loaded configuration
    ///
    font_available = false;
    font = new C2DFont();
    c2d::config::Group *fntGroup = config->getGroup("FONT");
    if (useZippedSkin) {
        int size = 0;
        font_data = getZippedData(path, fntGroup->getOption("path")->getString(), &size);
        if (font_data) {
            if (font->loadFromMemory(font_data, (size_t) size)) {
                font_available = true;
            }
        }
    } else {
        std::string fontPath = path + fntGroup->getOption("path")->getString();
        printf("font path: %s\n", fontPath.c_str());
        if (font->loadFromFile(fontPath)) {
            font_available = true;
        }
    }
    if (!font_available) {
        delete (font);
        font = ui->getFont();
    }
    font->setFilter((Texture::Filter) fntGroup->getOption("filtering")->getInteger());
    font->setOffset(fntGroup->getOption("offset")->getVector2f());
}

config::Config *Skin::getConfig() {
    return config;
}

config::Group Skin::createRectangleShapeGroup(const std::string &name,
                                              const c2d::FloatRect &rect,
                                              const c2d::Origin &origin,
                                              const std::string &texture,
                                              const c2d::Color &color,
                                              const c2d::Color &outlineColor, int outlineSize, Vector2f scale) {
    config::Group group(name);
    group.addOption({"texture", texture});
    group.addOption({"filtering", 1});
    group.addOption({"color", color});
    group.addOption({"outline_color", outlineColor});
    group.addOption({"outline_size", outlineSize});
    group.addOption({"rectangle", rect});
    group.addOption({"origin", (int) origin});
    group.addOption({"scaling", scale});
    return group;
}

Skin::RectangleShapeGroup Skin::getRectangleShape(const std::vector<std::string> &tree) {

    config::Option *option;
    RectangleShapeGroup rectangleShapeGroup{};

    c2d::config::Group *group = config->getGroup(tree[0]);
    if (!group) {
        printf("Skin::getRectangleShape: config group not found: \"%s\"\n", tree[0].c_str());
        return rectangleShapeGroup;
    }

    if (tree.size() > 1) {
        for (unsigned int i = 1; i < tree.size(); i++) {
            group = group->getGroup(tree[i]);
            if (!group) {
                printf("Skin::getRectangleShape: config group not found: \"%s\"\n", tree[i].c_str());
                return rectangleShapeGroup;
            }
        }
    }

    option = group->getOption("texture");
    if (option) {
        rectangleShapeGroup.texture = option->getString();
    }
    option = group->getOption("filtering");
    if (option) {
        rectangleShapeGroup.filtering = option->getInteger();
    }
    option = group->getOption("color");
    if (option) {
        rectangleShapeGroup.color = option->getColor();
    }
    option = group->getOption("outline_color");
    if (option) {
        rectangleShapeGroup.outlineColor = option->getColor();
    }
    option = group->getOption("outline_size");
    if (option) {
        rectangleShapeGroup.outlineSize = option->getInteger();
    }
    option = group->getOption("scaling");
    if (option) {
        rectangleShapeGroup.scaling = option->getVector2f();
    }
    option = group->getOption("rectangle");
    if (option) {
        rectangleShapeGroup.rect = option->getFloatRect();
        rectangleShapeGroup.rect.left *= global_scaling.x;
        rectangleShapeGroup.rect.top *= global_scaling.y;
        rectangleShapeGroup.rect.width *= global_scaling.x;
        rectangleShapeGroup.rect.height *= global_scaling.y;
        rectangleShapeGroup.rect.width *= rectangleShapeGroup.scaling.x;
        rectangleShapeGroup.rect.height *= rectangleShapeGroup.scaling.y;
    }
    option = group->getOption("origin");
    if (option) {
        rectangleShapeGroup.origin = (Origin) option->getInteger();
    }

    rectangleShapeGroup.available = true;

    return rectangleShapeGroup;
}

void Skin::loadRectangleShape(c2d::RectangleShape *shape, const std::vector<std::string> &tree) {

    RectangleShapeGroup rectangleShapeGroup = getRectangleShape(tree);
    if (!rectangleShapeGroup.available) {
        return;
    }

    shape->setOrigin(rectangleShapeGroup.origin);

    if (rectangleShapeGroup.rect.left > 0) {
        shape->setPosition(rectangleShapeGroup.rect.left, shape->getPosition().y);
    }
    if (rectangleShapeGroup.rect.top > 0) {
        shape->setPosition(shape->getPosition().x, rectangleShapeGroup.rect.top);
    }
    if (rectangleShapeGroup.rect.width > 0) {
        shape->setSize(rectangleShapeGroup.rect.width, shape->getSize().y);
    }
    if (rectangleShapeGroup.rect.height > 0) {
        shape->setSize(shape->getSize().x, rectangleShapeGroup.rect.height);
    }

    C2DTexture *tex = nullptr;
    if (!rectangleShapeGroup.texture.empty()) {
        if (useZippedSkin) {
            int size = 0;
            char *data = getZippedData(path, rectangleShapeGroup.texture, &size);
            printf("Skin::loadRectangleShape(%s, %s)\n",
                   path.c_str(), rectangleShapeGroup.texture.c_str());
            if (data) {
                tex = new C2DTexture((const unsigned char *) data, size);
                free(data);
            } else {
                printf("Skin::loadRectangleShape(%s, %s): failed to load texture\n",
                       path.c_str(), rectangleShapeGroup.texture.c_str());
            }
        } else {
            std::string bg_path = path + rectangleShapeGroup.texture;
            if (ui->getIo()->exist(bg_path)) {
                tex = new C2DTexture(bg_path);
            }
        }
    }

    if (tex && tex->available) {
        tex->setScale(rectangleShapeGroup.rect.width / tex->getSize().x,
                      rectangleShapeGroup.rect.height / tex->getSize().y);
        tex->setFillColor(rectangleShapeGroup.color);
        tex->setOutlineColor(rectangleShapeGroup.outlineColor);
        tex->setOutlineThickness((float) rectangleShapeGroup.outlineSize);
        tex->setFilter((Texture::Filter) rectangleShapeGroup.filtering);
        shape->setFillColor(Color::Transparent);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0);
        shape->add(tex);
    } else {
        if (tex) {
            delete (tex);
        }
        shape->setFillColor(rectangleShapeGroup.color);
        shape->setOutlineColor(rectangleShapeGroup.outlineColor);
        shape->setOutlineThickness(rectangleShapeGroup.outlineSize);
    }
}

config::Group Skin::createTextGroup(const std::string &name, int size, const c2d::FloatRect &rect,
                                    const c2d::Origin &origin, const c2d::Color &color,
                                    const c2d::Color &outlineColor, int outlineSize,
                                    const c2d::Text::Overflow &overflow, Vector2f scale) {
    config::Group group(name);
    group.addOption({"string", ""});
    group.addOption({"size", size});
    group.addOption({"color", color});
    group.addOption({"outline_color", outlineColor});
    group.addOption({"outline_size", outlineSize});
    group.addOption({"rectangle", rect});
    group.addOption({"origin", (int) origin});
    group.addOption({"overflow", (int) overflow});
    group.addOption({"scaling", scale});
    return group;
}

Skin::TextGroup Skin::getText(const std::vector<std::string> &tree) {

    config::Option *option;
    TextGroup textGroup{};

    c2d::config::Group *group = config->getGroup(tree[0]);
    if (!group) {
        printf("Skin::getText: config group not found: \"%s\"\n", tree[0].c_str());
        return textGroup;
    }

    if (tree.size() > 1) {
        for (unsigned int i = 1; i < tree.size(); i++) {
            group = group->getGroup(tree[i]);
            if (!group) {
                printf("Skin::getText: config group not found: \"%s\"\n", tree[i].c_str());
                return textGroup;
            }
        }
    }

    option = group->getOption("string");
    if (option) {
        textGroup.text = option->getString();
    }
    option = group->getOption("size");
    if (option) {
        textGroup.size = (unsigned int) option->getInteger();
        textGroup.size = (unsigned int) ((float) textGroup.size * global_scaling.y);
    }
    option = group->getOption("color");
    if (option) {
        textGroup.color = option->getColor();
    }
    option = group->getOption("outline_color");
    if (option) {
        textGroup.outlineColor = option->getColor();
    }
    option = group->getOption("outline_size");
    if (option) {
        textGroup.outlineSize = option->getInteger();
    }
    option = group->getOption("origin");
    if (option) {
        textGroup.origin = (Origin) option->getInteger();
    }
    option = group->getOption("scaling");
    if (option) {
        textGroup.scaling = option->getVector2f();
    }
    option = group->getOption("rectangle");
    if (option) {
        textGroup.rect = option->getFloatRect();
        textGroup.rect.left *= global_scaling.x;
        textGroup.rect.top *= global_scaling.y;
        textGroup.rect.width *= global_scaling.x;
        textGroup.rect.height *= global_scaling.y;
        textGroup.rect.width *= textGroup.scaling.x;
        textGroup.rect.height *= textGroup.scaling.y;
    }
    option = group->getOption("overflow");
    if (option) {
        textGroup.overflow = (Text::Overflow) option->getInteger();
    }

    textGroup.available = true;

    return textGroup;
}

void Skin::loadText(c2d::Text *text, const std::vector<std::string> &tree) {

    TextGroup textGroup = getText(tree);
    if (!textGroup.available) {
        return;
    }

    if (!textGroup.text.empty()) {
        text->setString(textGroup.text);
    }
    text->setCharacterSize(textGroup.size);
    text->setFillColor(textGroup.color);
    text->setOutlineColor(textGroup.outlineColor);
    text->setOutlineThickness((float) textGroup.outlineSize);
    text->setOrigin(textGroup.origin);
    text->setPosition(textGroup.rect.left, textGroup.rect.top);
    text->setOverflow(textGroup.overflow);
    text->setSizeMax(textGroup.rect.width, textGroup.rect.height);
}

Skin::Button *Skin::getButton(int id) {

    for (auto &button : buttons) {
        if (button.id == id) {
            return &button;
        }
    }
    return nullptr;
}

c2d::Font *Skin::getFont() {
    return font;
}

char *Skin::getZippedData(const std::string &p, const std::string &name, int *size) {

    char *data = nullptr;

    unzFile zip = unzOpen(p.c_str());
    if (!zip) {
        printf("Skin::getZippedData(%s, %s): unzOpen failed\n", p.c_str(), name.c_str());
        return data;
    }

    if (unzGoToFirstFile(zip) == UNZ_OK) {
        do {
            if (unzOpenCurrentFile(zip) == UNZ_OK) {
                unz_file_info fileInfo;
                memset(&fileInfo, 0, sizeof(unz_file_info));
                if (unzGetCurrentFileInfo(zip, &fileInfo, nullptr, 0, nullptr, 0, nullptr, 0) == UNZ_OK) {
                    char *zipFileName = (char *) malloc(fileInfo.size_filename + 1);
                    unzGetCurrentFileInfo(zip, &fileInfo, zipFileName, fileInfo.size_filename + 1,
                                          nullptr, 0, nullptr, 0);
                    zipFileName[fileInfo.size_filename] = '\0';
                    if (name == zipFileName) {
                        data = (char *) malloc(fileInfo.uncompressed_size);
                        if (size) {
                            *size = (int) fileInfo.uncompressed_size;
                        }
                        unzReadCurrentFile(zip, data, (unsigned int) fileInfo.uncompressed_size);
                        free(zipFileName);
                        unzClose(zip);
                        return data;
                    }
                    free(zipFileName);
                }
                unzCloseCurrentFile(zip);
            }
        } while (unzGoToNextFile(zip) == UNZ_OK);
    }

    unzClose(zip);

    return data;
}

Skin::~Skin() {

    for (auto &button : buttons) {
        if (button.texture) {
            delete (button.texture);
        }
    }
    buttons.clear();

    if (font) {
        delete (font);
    }

    // delete font data if loaded from zipped skin
    if (font_data) {
        free(font_data);
    }

    if (config) {
        delete (config);
    }
}

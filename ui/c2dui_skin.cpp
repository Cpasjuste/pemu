//
// Created by cpasjuste on 23/11/16.
//

#include "c2dui.h"

Skin::Skin(UiMain *u, const std::vector<Button> &btns, const Vector2f &scaling) {

    ui = u;

    // try to load skin from data directory first
    path = ui->getIo()->getDataPath() + "skins/" + ui->getConfig()->get(Option::GUI_SKIN)->getValueString() + "/";
    if (!ui->getIo()->exist(path + "config.cfg")) {
        // now try from romfs
        printf("Skin::Skin: skin config do not exist, trying from romfs... (%s)\n", (path + "config.cfg").c_str());
        path = ui->getIo()->getRomFsPath() + "skins/" + ui->getConfig()->get(Option::GUI_SKIN)->getValueString() + "/";
        if (!ui->getIo()->exist(path + "config.cfg")) {
            // restore defaults
            printf("Skin::Skin: skin config do not exist, reverting to default skin (%s)\n",
                   (path + "config.cfg").c_str());
            ui->getConfig()->get(Option::GUI_SKIN)->setValueString("default");
            path = ui->getIo()->getRomFsPath() + "skins/default/";
        }
    }

    config = new config::Config("SKIN_CONFIG", path + "config.cfg");
    printf("Skin path: %s\n", path.c_str());

    // load buttons textures
    buttons = btns;
    for (auto &button: buttons) {
        std::string buttonPath = path + "buttons/" + std::to_string(button.id) + ".png";
        if (ui->getIo()->exist(buttonPath)) {
            button.texture = new C2DTexture(buttonPath);
            if (button.texture != nullptr && !button.texture->available) {
                delete (button.texture);
            }
        }
    }

    ///
    /// GENERAL
    ///
    config::Group gen("GENERAL");
    gen.addOption({"resolution", Vector2f{1280.0f, 720.0f}});
    gen.addOption({"global_scaling", Vector2f{1.0f, 1.0f}});
    config->addGroup(gen);

    /// FONT
    config::Group font_group("FONT");
    font_group.addOption({"path", "default.ttf"});
    font_group.addOption({"offset", Vector2f{0, -3}});
    font_group.addOption({"filtering", 0});
    config->addGroup(font_group);

    /// HIGHLIGHT
    config::Group highlight = createRectangleShapeGroup("HIGHLIGHT");
    config->addGroup(highlight);

    /// MESSAGEBOX
    config::Group mbox = createRectangleShapeGroup("MESSAGEBOX");
    config->addGroup(mbox);

    /// MAIN (ROM LIST)
    config::Group main = createRectangleShapeGroup("MAIN");
    // rom list title + text
    config::Group title = createRectangleShapeGroup("TITLE");
    title.addGroup(createTextGroup("TEXT"));
    main.addGroup(title);
    config::Group help = createRectangleShapeGroup("HELP");
    main.addGroup(help);
    config::Group romList = createRectangleShapeGroup("ROM_LIST");
    config::Group romItem = createTextGroup("TEXT");
    romItem.addOption({"color_missing", Color::White});
    romItem.addOption({"color_not_working", Color::White});
    romItem.addOption({"highlight_use_text_color", 0});
    romList.addGroup(romItem);
    main.addGroup(romList);
    //
    config::Group romSyno = createRectangleShapeGroup("ROM_SYNOPSIS");
    romSyno.addGroup(createTextGroup("TEXT", ui->getFontSize()));
    main.addGroup(romSyno);
    //
    config::Group romInfo = createRectangleShapeGroup("ROM_INFOS");
    romInfo.addGroup(createTextGroup("SYSTEM_TEXT"));
    romInfo.addGroup(createTextGroup("DEVELOPER_TEXT"));
    romInfo.addGroup(createTextGroup("EDITOR_TEXT"));
    romInfo.addGroup(createTextGroup("DATE_TEXT"));
    romInfo.addGroup(createTextGroup("GENRE_TEXT"));
    romInfo.addGroup(createTextGroup("PLAYERS_TEXT"));
    romInfo.addGroup(createTextGroup("RATING_TEXT"));
    romInfo.addGroup(createTextGroup("TOPSTAFF_TEXT"));
    romInfo.addGroup(createTextGroup("ROTATION_TEXT"));
    romInfo.addGroup(createTextGroup("RESOLUTION_TEXT"));
    romInfo.addGroup(createTextGroup("CLASSIFICATION_TEXT"));
    romInfo.addGroup(createTextGroup("CLONEOF_TEXT"));
    romInfo.addGroup(createTextGroup("FILENAME_TEXT"));
    main.addGroup(romInfo);
    //
    config::Group previewBox = createRectangleShapeGroup("ROM_IMAGE");
    previewBox.addGroup(createTextGroup("TEXT", ui->getFontSize()));
    main.addGroup(previewBox);
    config->addGroup(main);
    ///
    /// MAIN (ROM LIST) (END)
    ///

    ///
    /// OPTIONS_MENU
    ///
    config::Group options_menu = createRectangleShapeGroup("OPTIONS_MENU");
    config::Group options_menu_title = createTextGroup("TITLE_TEXT");
    options_menu.addGroup(options_menu_title);
    config::Group options_menu_text = createTextGroup("ITEMS_TEXT");
    options_menu.addGroup(options_menu_text);
    config->addGroup(options_menu);
    ///
    /// OPTIONS_MENU (END)
    ///

    ///
    /// STATES_MENU
    ///
    config::Group states_menu = createRectangleShapeGroup("STATES_MENU");
    config::Group states_menu_title = createTextGroup("TITLE_TEXT");
    states_menu.addGroup(states_menu_title);
    config::Group states_item = createRectangleShapeGroup("STATES_ITEM");
    config::Group states_item_text = createTextGroup("STATES_TEXT");
    states_item.addGroup(states_item_text);
    states_menu.addGroup(states_item);
    config->addGroup(states_menu);
    ///
    /// STATES_MENU (END
    ///

    // load config
    config->load();

    ///
    /// load global scaling from loaded configuration
    ///
    c2d::config::Group *genGrp = config->getGroup("GENERAL");
    if (genGrp != nullptr) {
        c2d::config::Option *opt = genGrp->getOption("global_scaling");
        if (opt != nullptr) {
            global_scaling = opt->getVector2f();
            global_scaling.x *= (float) ui->getConfig()->get(c2dui::Option::Id::GUI_WINDOW_WIDTH)->getValueInt()
                                / (float) ui->getConfig()->get(c2dui::Option::Id::GUI_SCREEN_WIDTH)->getValueInt();
            global_scaling.y *= (float) ui->getConfig()->get(c2dui::Option::Id::GUI_WINDOW_HEIGHT)->getValueInt()
                                / (float) ui->getConfig()->get(c2dui::Option::Id::GUI_SCREEN_HEIGHT)->getValueInt();
        }
        opt = genGrp->getOption("resolution");
        if (opt != nullptr) {
            Vector2f uiRes = ui->getSize();
            Vector2f skinRes = opt->getVector2f();
            global_scaling.x *= uiRes.x / skinRes.x;
            global_scaling.y *= uiRes.y / skinRes.y;
        }
    }
    if (global_scaling.x == 1 && global_scaling.y == 1) {
        global_scaling = scaling;
    }

    ///
    /// load skin font from loaded configuration
    ///
    font_available = false;
    font = new C2DFont();
    c2d::config::Group *fntGroup = config->getGroup("FONT");
    std::string fontPath = path + fntGroup->getOption("path")->getString();
    //printf("font path: %s\n", fontPath.c_str());
    if (font->loadFromFile(fontPath)) {
        font_available = true;
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
                                              const c2d::Color &outlineColor, float outlineSize, Vector2f scale) {
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
    if (group == nullptr) {
        printf("Skin::getRectangleShape: config group not found: \"%s\"\n", tree[0].c_str());
        return rectangleShapeGroup;
    }

    if (tree.size() > 1) {
        for (unsigned int i = 1; i < tree.size(); i++) {
            group = group->getGroup(tree[i]);
            if (group == nullptr) {
                printf("Skin::getRectangleShape: config group not found: \"%s\"\n", tree[i].c_str());
                return rectangleShapeGroup;
            }
        }
    }

    option = group->getOption("rectangle");
    if (option != nullptr) {
        if (option->getFloatRect().width <= 0 || option->getFloatRect().height <= 0) {
            return rectangleShapeGroup;
        }
        rectangleShapeGroup.rect = option->getFloatRect();
        rectangleShapeGroup.rect.left *= global_scaling.x;
        rectangleShapeGroup.rect.top *= global_scaling.y;
        rectangleShapeGroup.rect.width *= global_scaling.x;
        rectangleShapeGroup.rect.height *= global_scaling.y;
        rectangleShapeGroup.rect.width *= rectangleShapeGroup.scaling.x;
        rectangleShapeGroup.rect.height *= rectangleShapeGroup.scaling.y;
    }
    option = group->getOption("texture");
    if (option != nullptr) {
        rectangleShapeGroup.texture = option->getString();
    }
    option = group->getOption("filtering");
    if (option != nullptr) {
        rectangleShapeGroup.filtering = option->getInteger();
    }
    option = group->getOption("color");
    if (option != nullptr) {
        rectangleShapeGroup.color = option->getColor();
    }
    option = group->getOption("outline_color");
    if (option != nullptr) {
        rectangleShapeGroup.outlineColor = option->getColor();
    }
    option = group->getOption("outline_size");
    if (option != nullptr) {
        rectangleShapeGroup.outlineSize = option->getFloat();
        rectangleShapeGroup.outlineSize *= global_scaling.y;
        if (rectangleShapeGroup.outlineSize > 0 && rectangleShapeGroup.outlineSize < 1) {
            rectangleShapeGroup.outlineSize = 1;
        }
    }
    option = group->getOption("scaling");
    if (option != nullptr) {
        rectangleShapeGroup.scaling = option->getVector2f();
    }
    option = group->getOption("origin");
    if (option != nullptr) {
        rectangleShapeGroup.origin = (Origin) option->getInteger();
    }

    rectangleShapeGroup.available = true;

    return rectangleShapeGroup;
}

bool
Skin::loadRectangleShape(c2d::RectangleShape *shape, const std::vector<std::string> &tree, bool textureUseFillColors) {

    RectangleShapeGroup rectangleShapeGroup = getRectangleShape(tree);
    if (!rectangleShapeGroup.available) {
        return false;
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
        std::string bg_path = path + rectangleShapeGroup.texture;
        if (ui->getIo()->exist(bg_path)) {
            tex = new C2DTexture(bg_path);
        }
    }

    if (tex && tex->available) {
        tex->setScale(rectangleShapeGroup.rect.width / tex->getSize().x,
                      rectangleShapeGroup.rect.height / tex->getSize().y);
        tex->setFillColor(rectangleShapeGroup.color);
        tex->setOutlineColor(rectangleShapeGroup.outlineColor);
        tex->setOutlineThickness(rectangleShapeGroup.outlineSize);
        tex->setFilter((Texture::Filter) rectangleShapeGroup.filtering);
        shape->setFillColor(textureUseFillColors ? rectangleShapeGroup.color : Color::Transparent);
        shape->setOutlineColor(Color::Transparent);
        shape->setOutlineThickness(0);
        shape->add(tex);
    } else {
        if (tex) delete (tex);
        shape->setFillColor(rectangleShapeGroup.color);
        shape->setOutlineColor(rectangleShapeGroup.outlineColor);
        shape->setOutlineThickness(rectangleShapeGroup.outlineSize);
    }

    return true;
}

config::Group Skin::createTextGroup(const std::string &name, int size, const c2d::FloatRect &rect,
                                    const c2d::Origin &origin, const c2d::Color &color,
                                    const c2d::Color &outlineColor, float outlineSize,
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

    config::Option *option = nullptr;
    TextGroup textGroup{};

    c2d::config::Group *group = config->getGroup(tree[0]);
    if (group == nullptr) {
        printf("Skin::getText: config group not found: \"%s\"\n", tree[0].c_str());
        return textGroup;
    }

    if (tree.size() > 1) {
        for (unsigned int i = 1; i < tree.size(); i++) {
            group = group->getGroup(tree[i]);
            if (group == nullptr) {
                printf("Skin::getText: config group not found: \"%s\"\n", tree[i].c_str());
                return textGroup;
            }
        }
    }

    option = group->getOption("size");
    if (option != nullptr) {
        if (option->getInteger() <= 0) {
            return textGroup;
        }
        textGroup.size = (unsigned int) ((float) option->getInteger() * global_scaling.y);
    }
    option = group->getOption("string");
    if (option != nullptr) {
        textGroup.text = option->getString();
    }
    option = group->getOption("color");
    if (option != nullptr) {
        textGroup.color = option->getColor();
    }
    option = group->getOption("outline_color");
    if (option != nullptr) {
        textGroup.outlineColor = option->getColor();
    }
    option = group->getOption("outline_size");
    if (option != nullptr) {
        textGroup.outlineSize = option->getFloat();
        textGroup.outlineSize *= global_scaling.y;
        if (textGroup.outlineSize > 0 && textGroup.outlineSize < 1) {
            textGroup.outlineSize = 1;
        }
    }
    option = group->getOption("origin");
    if (option != nullptr) {
        textGroup.origin = (Origin) option->getInteger();
    }
    option = group->getOption("scaling");
    if (option != nullptr) {
        textGroup.scaling = option->getVector2f();
    }
    option = group->getOption("rectangle");
    if (option != nullptr) {
        textGroup.rect = option->getFloatRect();
        textGroup.rect.left *= global_scaling.x;
        textGroup.rect.top *= global_scaling.y;
        textGroup.rect.width *= global_scaling.x;
        textGroup.rect.height *= global_scaling.y;
        textGroup.rect.width *= textGroup.scaling.x;
        textGroup.rect.height *= textGroup.scaling.y;
    }
    option = group->getOption("overflow");
    if (option != nullptr) {
        textGroup.overflow = (Text::Overflow) option->getInteger();
    }

    textGroup.available = true;

    return textGroup;
}

bool Skin::loadText(c2d::Text *text, const std::vector<std::string> &tree) {

    TextGroup textGroup = getText(tree);
    if (!textGroup.available) {
        return false;
    }

    text->setFont(getFont());
    text->setString(textGroup.text);
    text->setCharacterSize(textGroup.size);
    text->setFillColor(textGroup.color);
    text->setOutlineColor(textGroup.outlineColor);
    text->setOutlineThickness(textGroup.outlineSize);
    text->setOrigin(textGroup.origin);
    text->setPosition(textGroup.rect.left, textGroup.rect.top);
    text->setOverflow(textGroup.overflow);
    text->setSizeMax(textGroup.rect.width, textGroup.rect.height);

    return true;
}

Skin::Button *Skin::getButton(int id) {

    for (auto &button: buttons) {
        if (button.id == id) {
            return &button;
        }
    }
    return nullptr;
}

c2d::Font *Skin::getFont() {
    return font;
}

Skin::~Skin() {

    for (auto &button: buttons) {
        if (button.texture != nullptr) {
            delete (button.texture);
        }
    }
    buttons.clear();

    if (font != nullptr) {
        delete (font);
    }

    // delete font data if loaded from zipped skin
    if (font_data != nullptr) {
        free(font_data);
    }

    if (config != nullptr) {
        delete (config);
    }
}

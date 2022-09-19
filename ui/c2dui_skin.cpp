//
// Created by cpasjuste on 23/11/16.
//

#include "c2dui.h"

Skin::Skin(UiMain *u) {
    ui = u;

    // try to load skin from data directory first
    path = ui->getIo()->getDataPath() + "skins/" + ui->getConfig()->get(Option::GUI_SKIN)->getValueString() + "/";
    if (!ui->getIo()->exist(path + "config.cfg")) {
        // now try from romfs
        path = ui->getIo()->getRomFsPath() + "skins/" + ui->getConfig()->get(Option::GUI_SKIN)->getValueString() + "/";
        printf("Skin::Skin: skin config do not exist, trying from romfs... (%s)\n", (path + "config.cfg").c_str());
        if (!ui->getIo()->exist(path + "config.cfg")) {
            // restore defaults
            ui->getConfig()->get(Option::GUI_SKIN)->setValueString("default");
            path = ui->getIo()->getRomFsPath() + "skins/default/";
            printf("Skin::Skin: skin config do not exist, reverting to default skin (%s)\n",
                   (path + "config.cfg").c_str());
        }
    }

    config = new config::Config("SKIN_CONFIG", path + "config.cfg");
    printf("Skin path: %s\n", path.c_str());

    // load buttons textures
    // see c2d.h for key id
    buttons.emplace_back(KEY_JOY_UP_DEFAULT, "UP");
    buttons.emplace_back(KEY_JOY_DOWN_DEFAULT, "DOWN");
    buttons.emplace_back(KEY_JOY_LEFT_DEFAULT, "LEFT");
    buttons.emplace_back(KEY_JOY_RIGHT_DEFAULT, "RIGHT");
    buttons.emplace_back(KEY_JOY_A_DEFAULT, "A");
    buttons.emplace_back(KEY_JOY_B_DEFAULT, "B");
    buttons.emplace_back(KEY_JOY_X_DEFAULT, "X");
    buttons.emplace_back(KEY_JOY_Y_DEFAULT, "Y");
    buttons.emplace_back(KEY_JOY_LT_DEFAULT, "LT");
    buttons.emplace_back(KEY_JOY_RT_DEFAULT, "RT");
    buttons.emplace_back(KEY_JOY_LB_DEFAULT, "LB");
    buttons.emplace_back(KEY_JOY_RB_DEFAULT, "RB");
    buttons.emplace_back(KEY_JOY_LS_DEFAULT, "LS");
    buttons.emplace_back(KEY_JOY_RS_DEFAULT, "RS");
    buttons.emplace_back(KEY_JOY_SELECT_DEFAULT, "SELECT");
    buttons.emplace_back(KEY_JOY_START_DEFAULT, "START");
    buttons.emplace_back(KEY_JOY_MENU1_DEFAULT, "LB");
    buttons.emplace_back(KEY_JOY_MENU2_DEFAULT, "RB");
    buttons.emplace_back(100, "DPAD"); // dpad for help
    for (auto &button: buttons) {
        std::string buttonPath = path + "buttons/" + std::to_string(button.id) + ".png";
        if (ui->getIo()->exist(buttonPath)) {
            button.texture = new C2DTexture(buttonPath);
            if (button.texture && !button.texture->available) {
                delete (button.texture);
                button.texture = nullptr;
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
    font_group.addOption({"offset", Vector2f{0, -4}});
    font_group.addOption({"filtering", 0});
    config->addGroup(font_group);

    /// COLORS
    config::Group colors_group("COLORS");
    for (int i = 0; i < 50; i++) {
        colors_group.addOption({"color" + std::to_string(i), Color()});
    }
    config->addGroup(colors_group);

    /// HIGHLIGHT
    config::Group highlight = createRectangleShapeGroup("HIGHLIGHT");
    config->addGroup(highlight);

    /// MESSAGEBOX
    config::Group mbox = createRectangleShapeGroup("MESSAGEBOX");
    config->addGroup(mbox);

    /// STATUSBOX
    config::Group statusBox = createRectangleShapeGroup("STATUSBOX");
    config->addGroup(statusBox);

    /// MAIN (ROM LIST)
    config::Group main = createRectangleShapeGroup("MAIN");
    // rom list title + text
    config::Group title = createRectangleShapeGroup("TITLE");
    title.addGroup(createTextGroup("TEXT"));
    main.addGroup(title);
    config::Group help = createRectangleShapeGroup("HELP");
    main.addGroup(help);
    config::Group romList = createRectangleShapeGroup("ROM_LIST");
    config::Group romTitleItem = createTextGroup("SYSTEM_TEXT");
    config::Group romItem = createTextGroup("TEXT");
    romItem.addOption({"color_missing", Color::Red});
    romItem.addOption({"highlight_use_text_color", 0});
    romList.addGroup(romItem);
    romList.addGroup(romTitleItem);
    main.addGroup(romList);
    //
    config::Group romSyno = createRectangleShapeGroup("ROM_SYNOPSIS");
    romSyno.addGroup(createTextGroup("TEXT"));
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
    previewBox.addGroup(createTextGroup("TEXT"));
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

    // load config override
    config->load(config->getPath() + ".override");

    ///
    /// load global scaling from loaded configuration
    ///
    c2d::config::Group *genGrp = config->getGroup("GENERAL");
    if (genGrp) {
        c2d::config::Option *opt = genGrp->getOption("global_scaling");
        if (opt) {
            m_scaling = opt->getVector2f();
        }
        opt = genGrp->getOption("resolution");
        if (opt) {
            Vector2f uiRes = ui->getSize();
            Vector2f skinRes = opt->getVector2f();
            m_scaling.x *= uiRes.x / skinRes.x;
            m_scaling.y *= uiRes.y / skinRes.y;
        }
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
    Vector2f offset = fntGroup->getOption("offset")->getVector2f();
#ifdef __3DS__
    offset.y += 2;
#endif
    font->setOffset(offset);
}

config::Config *Skin::getConfig() {
    return config;
}

config::Group Skin::createRectangleShapeGroup(const std::string &name) {
    config::Group group(name);
    group.addOption({"texture", ""});
    group.addOption({"filtering", 1});
    group.addOption({"color", Color()});
    group.addOption({"outline_color", Color()});
    group.addOption({"outline_size", (float) 0});
    group.addOption({"rectangle", FloatRect()});
    group.addOption({"origin", (int) Origin::TopLeft});
    group.addOption({"scaling", Vector2f(1, 1)});
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

    option = group->getOption("rectangle");
    if (option) {
        if (option->getFloatRect().width <= 0 || option->getFloatRect().height <= 0) {
            return rectangleShapeGroup;
        }
        rectangleShapeGroup.rect = option->getFloatRect();
        rectangleShapeGroup.rect.left *= m_scaling.x;
        rectangleShapeGroup.rect.top *= m_scaling.y;
        rectangleShapeGroup.rect.width *= m_scaling.x;
        rectangleShapeGroup.rect.height *= m_scaling.y;
        rectangleShapeGroup.rect.width *= rectangleShapeGroup.scaling.x;
        rectangleShapeGroup.rect.height *= rectangleShapeGroup.scaling.y;
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
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            config::Option *opt = config->getOption("COLORS", option->getString());
            if (opt) {
                rectangleShapeGroup.color = opt->getColor();
                option->setType(config::Option::Type::Color);
                option->setColor(opt->getColor());
            }
        } else {
            rectangleShapeGroup.color = option->getColor();
        }
    }

    option = group->getOption("outline_color");
    if (option) {
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            config::Option *opt = config->getOption("COLORS", option->getString());
            if (opt) {
                rectangleShapeGroup.outlineColor = opt->getColor();
                option->setType(config::Option::Type::Color);
                option->setColor(opt->getColor());
            }
        } else {
            rectangleShapeGroup.outlineColor = option->getColor();
        }
    }

    option = group->getOption("outline_size");
    if (option) {
        rectangleShapeGroup.outlineSize = option->getFloat();
        rectangleShapeGroup.outlineSize *= m_scaling.y;
        if (rectangleShapeGroup.outlineSize > 0 && rectangleShapeGroup.outlineSize < 1) {
            rectangleShapeGroup.outlineSize = 1;
        }
    }

    option = group->getOption("scaling");
    if (option) {
        rectangleShapeGroup.scaling = option->getVector2f();
    }

    option = group->getOption("origin");
    if (option) {
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

    C2DTexture *tex;
    if (!rectangleShapeGroup.texture.empty()) {
        std::string bg_path = path + rectangleShapeGroup.texture;
        if (ui->getIo()->exist(bg_path)) {
            tex = new C2DTexture(bg_path);
            if (tex->available) {
                tex->setFilter((Texture::Filter) rectangleShapeGroup.filtering);
                tex->setScale(rectangleShapeGroup.rect.width / tex->getSize().x,
                              rectangleShapeGroup.rect.height / tex->getSize().y);
                shape->setTexture(tex, true);
            } else {
                delete (tex);
            }
        }
    }

    shape->setFillColor(rectangleShapeGroup.color);
    shape->setOutlineColor(rectangleShapeGroup.outlineColor);
    shape->setOutlineThickness(rectangleShapeGroup.outlineSize);

    return true;
}

config::Group Skin::createTextGroup(const std::string &name, int size) {
    config::Group group(name);
    group.addOption({"string", ""});
    group.addOption({"size", size});
    group.addOption({"color", Color()});
    group.addOption({"outline_color", Color()});
    group.addOption({"outline_size", (float) 0});
    group.addOption({"rectangle", FloatRect()});
    group.addOption({"origin", (int) Origin::TopLeft});
    group.addOption({"overflow", (int) c2d::Text::Overflow::Clamp});
    group.addOption({"scaling", Vector2f(1, 1)});
    return group;
}

Skin::TextGroup Skin::getText(const std::vector<std::string> &tree) {
    config::Option *option = nullptr;
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

    option = group->getOption("size");
    if (option) {
        if (option->getInteger() <= 0) {
            return textGroup;
        }
        textGroup.size = (unsigned int) ((float) option->getInteger() * m_scaling.y);
    }
    option = group->getOption("string");
    if (option) {
        textGroup.text = option->getString();
    }
    option = group->getOption("color");
    if (option) {
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            config::Option *opt = config->getOption("COLORS", option->getString());
            if (opt) {
                textGroup.color = opt->getColor();
                option->setType(config::Option::Type::Color);
                option->setColor(opt->getColor());
            }
        } else {
            textGroup.color = option->getColor();
        }
    }
    option = group->getOption("outline_color");
    if (option) {
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            config::Option *opt = config->getOption("COLORS", option->getString());
            if (opt) {
                textGroup.outlineColor = opt->getColor();
                option->setType(config::Option::Type::Color);
                option->setColor(opt->getColor());
            }
        } else {
            textGroup.outlineColor = option->getColor();
        }
    }
    option = group->getOption("outline_size");
    if (option) {
        textGroup.outlineSize = option->getFloat();
        textGroup.outlineSize *= m_scaling.y;
        textGroup.outlineSize = ceil(textGroup.outlineSize);
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
        textGroup.rect.left *= m_scaling.x;
        textGroup.rect.top *= m_scaling.y;
        textGroup.rect.width *= m_scaling.x;
        textGroup.rect.height *= m_scaling.y;
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

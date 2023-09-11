//
// Created by cpasjuste on 23/11/16.
//

#include "c2dui.h"
#include "c2dui_skin.h"

Skin::Skin(UiMain *u) {
    pMain = u;

    // setup skin paths (data path first then romfs path)
    mSkinPath[0] = pMain->getIo()->getDataPath() + "skins/"
                   + pMain->getConfig()->get(Option::GUI_SKIN)->getValueString() + "/";
    mSkinPath[1] = pMain->getIo()->getRomFsPath() + "skins/" +
                   pMain->getConfig()->get(Option::GUI_SKIN)->getValueString() + "/";

    // try to load skin from data directory first
    if (pMain->getIo()->exist(mSkinPath[0] + "config.cfg")) {
        pConfig = new config::Config("SKIN_CONFIG", mSkinPath[0] + "config.cfg");
        printf("Skin: config loaded from data dir (%s)\n", (mSkinPath[0] + "config.cfg").c_str());
    } else if (pMain->getIo()->exist(mSkinPath[1] + "config.cfg")) {
        pConfig = new config::Config("SKIN_CONFIG", mSkinPath[1] + "config.cfg");
        printf("Skin: config loaded from romfs (%s)\n", (mSkinPath[1] + "config.cfg").c_str());
    } else {
        // restore defaults
        pMain->getConfig()->get(Option::GUI_SKIN)->setValueString("default");
        mSkinPath[0] = mSkinPath[1] = pMain->getIo()->getRomFsPath() + "skins/default/";
        printf("Skin: config file not found, reverted to default skin (%s)\n",
               (mSkinPath[0] + "config.cfg").c_str());
        pConfig = new config::Config("SKIN_CONFIG", mSkinPath[0] + "config.cfg");
    }

    // get default font scaling
    mFontScaling = (float) pMain->getConfig()->get(Option::GUI_FONT_SCALING)->getValueInt(0);
    mFontScaling = mFontScaling > 0 ? 1 + (mFontScaling / 10) : 1;

    // load buttons textures
    // see c2d.h for key id
    mButtons.emplace_back(KEY_JOY_UP_DEFAULT, "UP");
    mButtons.emplace_back(KEY_JOY_DOWN_DEFAULT, "DOWN");
    mButtons.emplace_back(KEY_JOY_LEFT_DEFAULT, "LEFT");
    mButtons.emplace_back(KEY_JOY_RIGHT_DEFAULT, "RIGHT");
    mButtons.emplace_back(KEY_JOY_A_DEFAULT, "A");
    mButtons.emplace_back(KEY_JOY_B_DEFAULT, "B");
    mButtons.emplace_back(KEY_JOY_X_DEFAULT, "X");
    mButtons.emplace_back(KEY_JOY_Y_DEFAULT, "Y");
    mButtons.emplace_back(KEY_JOY_LT_DEFAULT, "LT");
    mButtons.emplace_back(KEY_JOY_RT_DEFAULT, "RT");
    mButtons.emplace_back(KEY_JOY_LB_DEFAULT, "LB");
    mButtons.emplace_back(KEY_JOY_RB_DEFAULT, "RB");
    mButtons.emplace_back(KEY_JOY_LS_DEFAULT, "LS");
    mButtons.emplace_back(KEY_JOY_RS_DEFAULT, "RS");
    mButtons.emplace_back(KEY_JOY_SELECT_DEFAULT, "SELECT");
    mButtons.emplace_back(KEY_JOY_START_DEFAULT, "START");
    mButtons.emplace_back(KEY_JOY_MENU1_DEFAULT, "LB");
    mButtons.emplace_back(KEY_JOY_MENU2_DEFAULT, "RB");
    mButtons.emplace_back(100, "DPAD"); // dpad for help
    for (auto &button: mButtons) {
        for (const auto &path: mSkinPath) {
            std::string buttonPath = path + "buttons/" + std::to_string(button.id) + ".png";
            if (pMain->getIo()->exist(buttonPath)) {
                button.texture = new C2DTexture(buttonPath);
                if (!button.texture->available) {
                    delete (button.texture);
                    button.texture = nullptr;
                }
                break;
            }
        }
    }

    ///
    /// GENERAL
    ///
    config::Group gen("GENERAL");
    gen.addOption({"resolution", Vector2f{1280.0f, 720.0f}});
    pConfig->addGroup(gen);

    /// FONT
    config::Group font_group("FONT");
    font_group.addOption({"path", "default.ttf"});
    font_group.addOption({"offset", Vector2f{0, -4}});
    font_group.addOption({"texture_filtering", 0});
    pConfig->addGroup(font_group);

    /// COLORS
    config::Group colors_group("COLORS");
    for (int i = 0; i < 50; i++) {
        colors_group.addOption({"color" + std::to_string(i), Color()});
    }
    pConfig->addGroup(colors_group);

    /// HIGHLIGHT
    config::Group highlight = createRectangleShapeGroup("HIGHLIGHT");
    pConfig->addGroup(highlight);

    /// MESSAGEBOX
    config::Group mbox = createRectangleShapeGroup("MESSAGEBOX");
    pConfig->addGroup(mbox);

    /// STATUSBOX
    config::Group statusBox = createRectangleShapeGroup("STATUSBOX");
    pConfig->addGroup(statusBox);

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
    pConfig->addGroup(main);
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
    pConfig->addGroup(options_menu);
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
    pConfig->addGroup(states_menu);
    ///
    /// STATES_MENU (END
    ///

    // load config
    printf("Skin: loading skin configuration (%s)\n", pConfig->getPath().c_str());
    pConfig->load();

    // load skin configuration override
    for (const auto &path: mSkinPath) {
        std::string cfgPath = path + "config.cfg.override";
        if (pMain->getIo()->exist(cfgPath)) {
            printf("Skin: loading configuration override (%s)\n", cfgPath.c_str());
            pConfig->load(cfgPath);
            break;
        }
    }

    // load 4/3 skin config override if needed
    if (pMain->getConfig()->get(Option::Id::GUI_SKIN_ASPECT)->getIndex() == 1) {
        for (const auto &path: mSkinPath) {
            std::string cfgPath = path + "config.cfg.override.43";
            if (pMain->getIo()->exist(cfgPath)) {
                printf("Skin: loading configuration override for 4/3 ar (%s)\n", cfgPath.c_str());
                pConfig->load(cfgPath);
                break;
            }
        }
    }

    ///
    /// compute global scaling from loaded configuration
    ///
    c2d::config::Group *genGrp = pConfig->getGroup("GENERAL");
    if (genGrp) {
        c2d::config::Option *opt = genGrp->getOption("resolution");
        if (opt) {
            Vector2f uiRes = pMain->getSize();
            Vector2f skinRes = opt->getVector2f();
            mScaling.x *= uiRes.x / skinRes.x;
            mScaling.y *= uiRes.y / skinRes.y;
        }
    }

    ///
    /// load skin font from loaded configuration
    ///
    auto fntGroup = pConfig->getGroup("FONT");
    for (const auto &path: mSkinPath) {
        std::string fontPath = path + fntGroup->getOption("path")->getString();
        if (pMain->getIo()->exist(fontPath)) {
            pFont = new C2DFont();
            pFont->loadFromFile(fontPath);
            break;
        }
    }

    getFont()->setFilter((Texture::Filter) fntGroup->getOption("texture_filtering")->getInteger());
    Vector2f offset = fntGroup->getOption("offset")->getVector2f();
#ifdef __3DS__
    offset.y += 1.6f;
#endif
    getFont()->setOffset(offset);
}

config::Config *Skin::getConfig() {
    return pConfig;
}

config::Group *Skin::getGroup(const std::vector<std::string> &tree) {
    config::Group *group = nullptr;

    if (tree.empty()) {
        return group;
    }

    group = pConfig->getGroup(tree[0]);
    if (!group) {
        printf("Skin::getGroup: config group not found: \"%s\"\n", tree[0].c_str());
        return group;
    }

    if (tree.size() > 1) {
        for (unsigned int i = 1; i < tree.size(); i++) {
            group = group->getGroup(tree[i]);
            if (!group) {
                printf("Skin::getGroup: config group not found: \"%s\"\n", tree[i].c_str());
                return nullptr;
            }
        }
    }

    return group;
}

config::Group Skin::createRectangleShapeGroup(const std::string &name) {
    config::Group group(name);
    group.addOption({"texture", ""});
    group.addOption({"texture_ratio", "free"});
    group.addOption({"texture_filtering", 1});
    group.addOption({"texture_color", Color()});
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

    c2d::config::Group *group = pConfig->getGroup(tree[0]);
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
        rectangleShapeGroup.rect.left *= mScaling.x;
        rectangleShapeGroup.rect.top *= mScaling.y;
        rectangleShapeGroup.rect.width *= mScaling.x;
        rectangleShapeGroup.rect.height *= mScaling.y;
        rectangleShapeGroup.rect.width *= rectangleShapeGroup.scaling.x;
        rectangleShapeGroup.rect.height *= rectangleShapeGroup.scaling.y;
    }

    option = group->getOption("texture");
    if (option) {
        rectangleShapeGroup.texture = option->getString();
    }

    option = group->getOption("texture_ratio");
    if (option && option->getString() != "free") {
        rectangleShapeGroup.textureRatio =
                option->getString() == "keep_width" ? RectangleShapeGroup::TextureRatio::KeepWidth
                                                    : RectangleShapeGroup::TextureRatio::KeepHeight;
    }

    option = group->getOption("texture_filtering");
    if (option) {
        rectangleShapeGroup.filtering = option->getInteger();
    }

    option = group->getOption("texture_color");
    if (option) {
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            config::Option *opt = pConfig->getOption("COLORS", option->getString());
            if (opt) {
                rectangleShapeGroup.color = opt->getColor();
                option->setType(config::Option::Type::Color);
                option->setColor(opt->getColor());
            }
        } else {
            rectangleShapeGroup.color = option->getColor();
        }
    }

    option = group->getOption("color");
    if (option) {
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            config::Option *opt = pConfig->getOption("COLORS", option->getString());
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
            config::Option *opt = pConfig->getOption("COLORS", option->getString());
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
        rectangleShapeGroup.outlineSize *= mScaling.y;
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
        for (const auto &path: mSkinPath) {
            std::string texPath = path + rectangleShapeGroup.texture;
            if (pMain->getIo()->exist(texPath)) {
                tex = new C2DTexture(texPath);
                if (tex->available) {
                    if (rectangleShapeGroup.textureRatio == RectangleShapeGroup::TextureRatio::KeepWidth) {
                        float scaling = rectangleShapeGroup.rect.width / tex->getSize().x;
                        tex->setScale(scaling, scaling);
                    } else if (rectangleShapeGroup.textureRatio == RectangleShapeGroup::TextureRatio::KeepHeight) {
                        float scaling = rectangleShapeGroup.rect.height / tex->getSize().y;
                        tex->setScale(scaling, scaling);
                    } else {
                        tex->setScale(rectangleShapeGroup.rect.width / tex->getSize().x,
                                      rectangleShapeGroup.rect.height / tex->getSize().y);
                    }
                    tex->setFilter((Texture::Filter) rectangleShapeGroup.filtering);
                    shape->add(tex);
                } else {
                    delete (tex);
                }
                break;
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

    c2d::config::Group *group = pConfig->getGroup(tree[0]);
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
        textGroup.size = (unsigned int) ((float) option->getInteger() * mScaling.y * mFontScaling);
    }
    option = group->getOption("string");
    if (option) {
        textGroup.text = option->getString();
    }
    option = group->getOption("color");
    if (option) {
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            config::Option *opt = pConfig->getOption("COLORS", option->getString());
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
            config::Option *opt = pConfig->getOption("COLORS", option->getString());
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
        textGroup.outlineSize *= mScaling.y;
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
        textGroup.rect.left *= mScaling.x;
        textGroup.rect.top *= mScaling.y;
        textGroup.rect.width *= mScaling.x;
        textGroup.rect.height *= mScaling.y;
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
    for (auto &button: mButtons) {
        if (button.id == id) {
            return &button;
        }
    }
    return nullptr;
}

Vector2f Skin::getScale(c2d::config::Group *group) {
    auto option = group->getOption("scaling");
    return option ? option->getVector2f() : Vector2f(1, 1);
}

FloatRect Skin::getRectangle(c2d::config::Group *group) {
    auto option = group->getOption("rectangle");
    auto rect = option ? option->getFloatRect() : FloatRect();
    if (option && rect.width > 0 && rect.height > 0) {
        auto local_scale = getScale(group);
        rect.left *= mScaling.x;
        rect.top *= mScaling.y;
        rect.width *= mScaling.x;
        rect.height *= mScaling.y;
        rect.width *= local_scale.x;
        rect.height *= local_scale.y;
    }

    return rect;
}

Color Skin::getColor(c2d::config::Group *group, const std::string &name) {
    auto option = group->getOption(name);
    if (option) {
        if (option->getType() == config::Option::Type::String) {
            // retrieve color from variable string
            auto opt = pConfig->getOption("COLORS", option->getString());
            if (opt) {
                return opt->getColor();
            }
        } else {
            return option->getColor();

        }
    }

    return name == "texture_color" ? Color::White : Color::Transparent;
}

Color Skin::getFillColor(c2d::config::Group *group) {
    return getColor(group, "color");
}

Color Skin::getOutlineColor(c2d::config::Group *group) {
    return getColor(group, "outline_color");
}

float Skin::getOutlineThickness(c2d::config::Group *group) {
    auto option = group->getOption("outline_size");
    if (option) {
        return option->getFloat() * mScaling.y;
        /* TODO: test this again to see if it's really needed...
        float outline = option->getFloat() * mScaling.y;
        if (outline > 0 && outline < 1) {
            outline = 1;
        }
        return outline;
        */
    }

    return 0;
}

Origin Skin::getOrigin(c2d::config::Group *group) {
    auto option = group->getOption("origin");
    if (option) {
        return (Origin) option->getInteger();
    }

    return Origin::TopLeft;
}

unsigned int Skin::getCharacterSize(c2d::config::Group *group) {
    auto option = group->getOption("size");
    if (option && option->getInteger() > 0) {
        auto local_scale = getScale(group);
        return (unsigned int) ((float) option->getInteger() * local_scale.y * getFontScaling());
    }

    return 0;
}

std::string Skin::getString(c2d::config::Group *group) {
    auto option = group->getOption("string");
    if (option) {
        return option->getString();
    }

    return {};
}

Text::Overflow Skin::getOverflow(c2d::config::Group *group) {
    auto option = group->getOption("overflow");
    if (option) {
        return (Text::Overflow) option->getInteger();
    }

    return Text::Overflow::Clamp;
}

c2d::Font *Skin::getFont() {
    return pFont ? pFont : pMain->getFont();
}

Skin::~Skin() {
    for (auto &button: mButtons) {
        delete (button.texture);
    }

    delete (pFont);
    delete (pConfig);
}

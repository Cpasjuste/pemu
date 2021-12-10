//
// Created by cpasjuste on 30/01/18.
//

#include "c2dui.h"

class MenuLine : public c2d::RectangleShape {

public:

    MenuLine(UIMain *u, FloatRect &rect, Skin::TextGroup &tg) : RectangleShape(rect) {

        ui = u;
        textGroup = tg;
        Font *font = ui->getSkin()->font;

        name = new Text("OPTION NAME", textGroup.size, font);
        name->setFillColor(textGroup.color);
        name->setOutlineThickness(textGroup.outlineSize);
        name->setOutlineColor(textGroup.outlineColor);
        name->setOrigin(Origin::Left);
        name->setPosition(0, MenuLine::getSize().y / 2);
        name->setSizeMax((MenuLine::getSize().x * 0.6f), 0);
        MenuLine::add(name);

        value = new Text("OPTION VALUE", textGroup.size, font);
        value->setFillColor(textGroup.color);
        value->setOutlineThickness(textGroup.outlineSize);
        value->setOutlineColor(textGroup.outlineColor);
        value->setOrigin(Origin::Left);
        value->setPosition((MenuLine::getSize().x * 0.6f), MenuLine::getSize().y / 2);
        value->setSizeMax(MenuLine::getSize().x * 0.3f, 0);
        MenuLine::add(value);

        sprite = new Sprite();
        MenuLine::add(sprite);
    }

    void setOption(const Option &opt) {
        // always hide sprite (icon) first
        if (sprite != nullptr) {
            sprite->setVisibility(Visibility::Hidden);
        }

        // reset
        name->setStyle(Text::Regular);
        value->setVisibility(Visibility::Visible);
        MenuLine::setFillColor(Color::Transparent);

        option = opt;
        name->setString(option.getName());

        if (option.getFlags() & Option::Flags::INPUT) {
            Skin::Button *button = ui->getSkin()->getButton(option.getValueInt());
            if (button != nullptr && option.getId() < Option::Id::JOY_DEADZONE) {
                if (button->texture != nullptr) {
                    sprite->setTexture(button->texture, true);
                    sprite->setVisibility(Visibility::Visible);
                    value->setVisibility(Visibility::Hidden);
                    float tex_scaling = std::min(
                            ((getSize().x * 0.8f)) / (float) sprite->getTextureRect().width,
                            ((getSize().y * 0.8f)) / (float) sprite->getTextureRect().height);
                    sprite->setScale(tex_scaling, tex_scaling);
                    sprite->setPosition((getSize().x * 0.67f), (getSize().y / 2) - 1);
                    sprite->setOrigin(Origin::Left);
                } else {
                    sprite->setVisibility(Visibility::Hidden);
                    value->setVisibility(Visibility::Visible);
                    value->setString(button->name);
                }
            } else {
                char btn[16];
                snprintf(btn, 16, "%i", option.getValueInt());
                value->setVisibility(Visibility::Visible);
                value->setString(btn);
            }
        } else if (option.getFlags() & Option::Flags::MENU) {
            name->setStyle(Text::Italic);
            value->setVisibility(Visibility::Hidden);
            MenuLine::setFillColor(ui->getUiMenu()->getOutlineColor());
        } else {
            value->setVisibility(Visibility::Visible);
            value->setString(option.getValueString());
        }
    }

    UIMain *ui = nullptr;
    Text *name = nullptr;
    Text *value = nullptr;
    Sprite *sprite = nullptr;
    Skin::TextGroup textGroup;
    Option option;
};

UIMenuNew::UIMenuNew(UIMain *uiMain) : RectangleShape(Vector2f(0, 0)) {

    ui = uiMain;
    Skin *skin = ui->getSkin();

    skin->loadRectangleShape(this, {"OPTIONS_MENU"});
    alpha = UIMenuNew::getAlpha();

    // TODO: use/update skin
    UIMenuNew::setSize({ui->getSize().x / 3, ui->getSize().y - 128});
    UIMenuNew::setOrigin(Origin::Left);
    UIMenuNew::setPosition({ui->getSize().x, ui->getSize().y / 2});

    // tween
    Vector2f targetPos = {UIMenuNew::getPosition().x - UIMenuNew::getSize().x,
                          UIMenuNew::getPosition().y};
    tweenPosition = new TweenPosition({UIMenuNew::getPosition()}, targetPos, 0.2f);
    tweenPosition->setState(TweenState::Stopped);
    UIMenuNew::add(tweenPosition);

    // retrieve skin config for an option item
    textGroup = ui->getSkin()->getText({"OPTIONS_MENU", "ITEMS_TEXT"});

    // calculate number of items shown
    lineHeight = (float) (textGroup.rect.height);
    maxLines = (int) (UIMenuNew::getSize().y / lineHeight);
    if ((float) maxLines * lineHeight < UIMenuNew::getSize().y) {
        lineHeight = UIMenuNew::getSize().y / (float) maxLines;
    }

    // add selection rectangle (highlight)
    highlight = new RectangleShape({16, 16});
    ui->getSkin()->loadRectangleShape(highlight, {"SKIN_CONFIG", "HIGHLIGHT"});
    highlight->setSize(UIMenuNew::getSize().x, lineHeight);
    UIMenuNew::add(highlight);

    // add options items
    for (unsigned int i = 0; i < (unsigned int) maxLines; i++) {
        FloatRect rect = {0, (lineHeight * (float) i), UIMenuNew::getSize().x, lineHeight};
        auto line = new MenuLine(ui, rect, textGroup);
        line->setLayer(2);
        lines.push_back(line);
        UIMenuNew::add(line);
    }

    // TODO: fix bluer
    // add blur
    blur = new RectangleShape(ui->getLocalBounds());
    blur->setFillColor(Color::Gray);
    blur->setLayer(0);
    blur->add(new TweenAlpha(0, 230, 0.3));
    blur->setVisibility(Visibility::Hidden);
    UIMenuNew::add(blur);

    // hide by default
    UIMenuNew::setLayer(1);
    UIMenuNew::setVisibility(Visibility::Hidden);

#if 0
    printf("GuiMenu (%p)\n", this);
    this->ui = ui;

    Skin *skin = ui->getSkin();

    skin->loadRectangleShape(this, {"OPTIONS_MENU"});
    alpha = RectangleShape::getAlpha();

    // menu title
    title = new Text("TITLE", C2D_DEFAULT_CHAR_SIZE, ui->getSkin()->font);
    title->setStyle(Text::Underlined);
    skin->loadText(title, {"OPTIONS_MENU", "TITLE_TEXT"});
    RectangleShape::add(title);

    textGroup = ui->getSkin()->getText({"OPTIONS_MENU", "ITEMS_TEXT"});
    int start_y = (int) textGroup.rect.top;
    // calculate lines per menu
    float line_height = ui->getSkin()->getFont()->getLineSpacing(textGroup.size) + 4;
    int max_lines = (int) ((RectangleShape::getSize().y - (float) start_y) / line_height) * 2;

    // add selection rectangle (highlight)
    highlight = new RectangleShape({16, 16});
    ui->getSkin()->loadRectangleShape(highlight, {"SKIN_CONFIG", "HIGHLIGHT"});
    highlight->setSize(((RectangleShape::getSize().x / 2) * 0.5f) - 4, line_height);
    RectangleShape::add(highlight);

    // add lines of text
    for (int i = 0; i < max_lines; i++) {
        FloatRect rect = {textGroup.rect.left, (float) start_y + ((float) i * line_height),
                          RectangleShape::getSize().x / 2, line_height};
        if (i >= max_lines / 2) {
            rect.left = RectangleShape::getSize().x / 2;
            rect.top = (float) start_y + (((float) i - ((float) max_lines / 2)) * line_height);
        }
        lines.push_back(new MenuLine(ui, rect, textGroup));
        RectangleShape::add(lines[i]);
    }

    // build menus
    optionMenuGui = new OptionMenu(nullptr, ui->getConfig()->get());
    optionMenuGui->addChild("EXIT");
    optionMenuRom = new OptionMenu(nullptr, ui->getConfig()->get(true), true);
    optionMenuRom->addChild("STATES", true);
    optionMenuRom->addChild("RETURN");
    optionMenuRom->addChild("EXIT");


    tweenPosition = new TweenPosition({RectangleShape::getPosition().x, -RectangleShape::getSize().y},
                                      RectangleShape::getPosition(), 0.2f);
    tweenPosition->setState(TweenState::Stopped);
    RectangleShape::add(tweenPosition);

    RectangleShape::setVisibility(Visibility::Hidden);
#endif
}

void UIMenuNew::load(bool isRom) {

    isRomMenu = isRom;
    isEmuRunning = ui->getUiEmu()->isVisible();

    options.clear();
    std::vector<Option> *opts = ui->getConfig()->get(isRomMenu);
    remove_copy_if(opts->begin(), opts->end(),
                   back_inserter(options), [this](Option &opt) {
                return isOptionHidden(&opt) || opt.getFlags() & Option::Flags::HIDDEN;
            });

    setAlpha(isEmuRunning ? (uint8_t) (alpha - 50) : (uint8_t) alpha);

    // update options lines/items
    updateLines();

    // finally, show me
    if (!isVisible()) {
        blur->setVisibility(Visibility::Visible, true);
        setVisibility(Visibility::Visible, true);
        setLayer(1);
    }

#if 0
    isRomMenu = isRom;
    options = isRomMenu ? ui->getConfig()->get(true)
                        : ui->getConfig()->get();

    if (om == nullptr) {
        optionMenu = isRomMenu ? optionMenuRom : optionMenuGui;
    } else {
        optionMenu = om;
    }

    isEmuRunning = ui->getUiEmu()->isVisible();
    setAlpha(isEmuRunning ? (uint8_t) (alpha - 50) : (uint8_t) alpha);

    optionIndex = optionMenu->parent || isRomMenu ? 0 : 1;
    optionCount = (int) (optionMenu->childs.size() + optionMenu->option_ids.size());

    if (isEmuRunning) {
        // if frameskip is enabled, we may get a black buffer,
        // force a frame to be drawn
        // not needed anymore.. no frameskip option
        // ui->getUiEmu()->updateFb();
    }

    if (isRomMenu) {
        title->setString(ui->getUiRomList()->getSelection().getName().text);
    } else {
        title->setString(optionMenu->title);
    }

    for (auto &line: lines) {
        line->setVisibility(Visibility::Hidden);
    }

    // add main options
    int line_index = 0;
    for (unsigned int i = 0; i < optionMenu->option_ids.size(); i++) {

        if (i >= lines.size()) {
            // oups
            break;
        }

        // menu types
        Option *option = ui->getConfig()->get(optionMenu->option_ids[i], isRomMenu);
        if (option == nullptr) {
            optionCount--;
            continue;
        }

        // skip rotation option if not needed
        if ((isOptionHidden(option)) || option->getFlags() & Option::Flags::HIDDEN) {
            optionCount--;
            continue;
        }

        lines[line_index]->setOption(option);
        lines[line_index]->setVisibility(Visibility::Visible);

        line_index++;
    }

    // add child's menus options
    for (unsigned i = 0; i < optionMenu->childs.size(); i++) {

        if (i >= lines.size()) {
            // oups
            break;
        }

        // don't show custom in-game options when a game is not running
        if (isRomMenu && !isEmuRunning) {
            std::string t = optionMenu->childs[i]->title;
            if (t == "EXIT" || t == "RETURN") {
                optionCount--;
                continue;
            }
        }

        lines[line_index]->setOption(nullptr);
        lines[line_index]->name->setString(optionMenu->childs[i]->title);
        lines[line_index]->setVisibility(Visibility::Visible);
        line_index++;
    }

    updateHighlight();

    if (!isVisible()) {
        setVisibility(Visibility::Visible, true);
        setLayer(1);
    }
#endif
}

void UIMenuNew::updateLines() {
    for (unsigned int i = 0; i < (unsigned int) maxLines; i++) {
        if (optionIndex + i >= options.size()) {
            lines[i]->setVisibility(Visibility::Hidden);
            continue;
        }
        const Option option = options.at(optionIndex + i);
        // set line data
        lines[i]->setOption(option);
        lines[i]->setVisibility(Visibility::Visible);
        // set highlight position and color
        if ((int) i == highlightIndex) {
            highlight->setPosition({highlight->getPosition().x, lines[i]->getPosition().y});
            lines[i]->value->setOutlineColor(getOutlineColor());
        } else {
            lines[i]->value->setOutlineColor(textGroup.outlineColor);
        }
    }
}

bool UIMenuNew::onInput(c2d::Input::Player *players) {

    unsigned int keys = players[0].keys;

    if (ui->getUiStateMenu()->isVisible()) {
        return C2DObject::onInput(players);
    }

    // UP
    if (keys & Input::Key::Up) {
        if (highlightIndex <= maxLines / 2 && optionIndex > 0) {
            optionIndex--;
        } else {
            highlightIndex--;
            if (highlightIndex < 0) {
                highlightIndex = maxLines / 2;
                if (highlightIndex >= (int) options.size()) {
                    highlightIndex = (int) options.size() - 1;
                    optionIndex = 0;
                } else {
                    optionIndex = ((int) options.size() - 1) - highlightIndex;
                }
            }
        }
        updateLines();
    }
    // DOWN
    if (keys & Input::Key::Down) {
        if (highlightIndex >= maxLines / 2) {
            optionIndex++;
            if (optionIndex + highlightIndex >= (int) options.size()) {
                optionIndex = 0;
                highlightIndex = 0;
            }
        } else {
            highlightIndex++;
            if (highlightIndex >= (int) options.size()) {
                highlightIndex = 0;
            }
        }
        updateLines();
    }
    // BACK
    if (keys & Input::Key::Start || keys & Input::Key::Select || keys & Input::Key::Fire2) {
        setVisibility(Visibility::Hidden, true);
        if (isEmuRunning) {
            ui->getUiEmu()->resume();
        } else {
            ui->getUiRomList()->setVisibility(Visibility::Visible);
        }
    }

#if 0
    bool option_changed = false;
    unsigned int keys = players[0].keys;

    // UP
    if (keys & Input::Key::Up) {
        optionIndex--;
        if (optionIndex < 0) {
            optionIndex = optionCount - 1;
        }
        Option *option = lines[optionIndex]->option;
        while (option && (option->getFlags() & Option::Flags::DELIMITER)) {
            optionIndex--;
            if (optionIndex < 0) {
                optionIndex = optionCount - 1;
            }
            option = lines[optionIndex]->option;
        }
        updateHighlight();
    }

    // DOWN
    if (keys & Input::Key::Down) {
        optionIndex++;
        if (optionIndex >= optionCount) {
            optionIndex = 0;
        }
        Option *option = lines[optionIndex]->option;
        while (option && (option->getFlags() & Option::Flags::DELIMITER)) {
            optionIndex++;
            if (optionIndex >= optionCount) {
                optionIndex = 0;
            }
            option = lines[optionIndex]->option;
        }

        updateHighlight();
    }

    // LEFT /RIGHT
    if ((keys & Input::Key::Left || keys & Input::Key::Right) &&
        (unsigned int) optionIndex < optionMenu->option_ids.size()) {
        Option *option = lines[optionIndex]->option;
        if (!option) {
            // should never happen
            return true;
        }
        option_changed = true;
        if (!(option->getFlags() & Option::Flags::MENU)) {
            if (keys & Input::Key::Left) {
                option->prev();
            } else {
                option->next();
            }
            lines[optionIndex]->setOption(option);

            if (!option->getInfo().empty()) {
                ui->getUiMessageBox()->show("WARNING", option->getInfo(), "OK");
            }

            switch (option->getId()) {
                case Option::Id::GUI_SHOW_ALL:
                case Option::Id::GUI_SHOW_ROM_NAMES:
                case Option::Id::GUI_FILTER_CLONES:
                case Option::Id::GUI_FILTER_SYSTEM:
                case Option::Id::GUI_FILTER_EDITOR:
                case Option::Id::GUI_FILTER_DEVELOPER:
                case Option::Id::GUI_FILTER_PLAYERS:
                case Option::Id::GUI_FILTER_RATING:
                case Option::Id::GUI_FILTER_ROTATION:
                case Option::Id::GUI_FILTER_RESOLUTION:
                case Option::Id::GUI_FILTER_DATE:
                case Option::Id::GUI_FILTER_GENRE:
                    ui->getUiRomList()->updateRomList();
                    break;

                case Option::ROM_ROTATION:
                case Option::Id::ROM_SCALING:
                    if (isEmuRunning) {
                        ui->getUiEmu()->getVideo()->updateScaling();
                    }
                    break;
                case Option::Id::ROM_FILTER:
                    if (isEmuRunning) {
                        ui->getUiEmu()->getVideo()->setFilter((Texture::Filter) option->getIndex());
                    }
                    break;
                case Option::Id::ROM_SHADER:
                    if (isEmuRunning) {
                        ui->getUiEmu()->getVideo()->setShader(option->getIndex());
                    }
                    break;
                case Option::Id::GUI_WINDOW_LEFT:
                    ui->setPosition((float) option->getValueInt(), ui->getPosition().y);
                    break;
                case Option::Id::GUI_WINDOW_TOP:
                    ui->setPosition(ui->getPosition().x, (float) option->getValueInt());
                    break;
                case Option::Id::GUI_WINDOW_WIDTH:
                    ui->setScale((float) option->getValueInt() / ui->getSize().x, ui->getScale().y);
                    break;
                case Option::Id::GUI_WINDOW_HEIGHT:
                    ui->setScale(ui->getScale().x, (float) option->getValueInt() / ui->getSize().y);
                    break;
                case Option::Id::GUI_VIDEO_SNAP_DELAY:
                    ui->getUiRomList()->setVideoSnapDelay(option->getValueInt());
                    break;
#ifdef __SWITCH__
                    case Option::Id::JOY_SINGLEJOYCON:
                        ((SWITCHInput *) ui->getInput())->setSingleJoyconMode(option->getValueBool());
                        break;
#endif
                default:
                    break;
            }
        }
    }

    // FIRE1
    if (keys & Input::Key::Fire1) {
        if ((unsigned int) optionIndex < optionMenu->option_ids.size()) {
            Option *option = lines[optionIndex]->option;
            if (option->getFlags() == Option::Flags::INPUT) {
                int new_key = 0;
                int res = ui->getUiMessageBox()->show("NEW INPUT", "PRESS A BUTTON", "", "", &new_key, 9);
                if (res != MessageBox::TIMEOUT) {
                    option->setValueInt(new_key);
                    option_changed = true;
                    lines[optionIndex]->setOption(option);
                }
            }
        } else {
            // extra options in menu (manually added)
            OptionMenu *menu = optionMenu->childs[optionIndex - optionMenu->option_ids.size()];
            if (menu->title == "EXIT") {
                setVisibility(Visibility::Hidden, true);
                if (isRomMenu) {
                    ui->getUiEmu()->stop();
                    ui->getUiRomList()->setVisibility(Visibility::Visible);
                    ui->getInput()->clear();
                } else {
                    ui->done = true;
                }
            } else if (menu->title == "STATES") {
                setVisibility(Visibility::Hidden, true);
                ui->getUiStateMenu()->setVisibility(Visibility::Visible, true);
            } else if (menu->title == "RETURN") {
                setVisibility(Visibility::Hidden, true);
                ui->getUiEmu()->resume();
            } else {
                load(isRomMenu, menu);
            }
        }
    }

    // FIRE2
    if (keys & Input::Key::Fire2
        || (!isEmuRunning && (keys & Input::Key::Start && !isRomMenu))
        || (!isEmuRunning && (keys & Input::Key::Select && isRomMenu))) {
        if (optionMenu->parent == nullptr) {
            if (isEmuRunning) {
                setVisibility(Visibility::Hidden, true);
                ui->getUiEmu()->resume();
            } else {
                setVisibility(Visibility::Hidden, true);
                ui->getUiRomList()->setVisibility(Visibility::Visible);
            }
        } else {
            load(isRomMenu, optionMenu->parent);
        }
    }

    if (keys & EV_QUIT) {
        ui->done = true;
    }

    if (option_changed) {
        updateHighlight();
        if (isRomMenu) {
            ui->getConfig()->save(ui->getUiRomList()->getSelection());
        } else {
            ui->getConfig()->save(ss_api::Game());
        }
    }
#endif
    return true;
}

UIMenuNew::~UIMenuNew() {
    printf("~UIMenuNew\n");
    //if (optionMenuGui) delete (optionMenuGui);
    //if (optionMenuRom) delete (optionMenuRom);
}

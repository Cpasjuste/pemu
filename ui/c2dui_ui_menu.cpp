//
// Created by cpasjuste on 30/01/18.
//

#ifdef __SWITCH__
extern "C" {
#include <switch/services/hid.h>
}
#endif

#include "c2dui.h"

using namespace c2d;
using namespace c2dui;

class MenuLine : public c2d::RectangleShape {

public:

    MenuLine(UIMain *u, FloatRect &rect, Skin::TextGroup &tg) : RectangleShape(rect) {

        setFillColor(Color::Transparent);

        ui = u;
        textGroup = tg;
        Font *font = ui->getSkin()->font;

        name = new Text("OPTION NAME,)'", textGroup.size, font);
        name->setFillColor(textGroup.color);
        name->setOutlineThickness(textGroup.outlineSize);
        name->setOutlineColor(textGroup.outlineColor);
        name->setOrigin(Origin::Left);
        name->setPosition(0, getSize().y / 2);
        name->setSizeMax((getSize().x * 0.50f), 0);
        add(name);

        value = new Text("OPTION VALUE,)'", textGroup.size, font);
        value->setFillColor(textGroup.color);
        value->setOutlineThickness(textGroup.outlineSize);
        value->setOutlineColor(textGroup.outlineColor);
        value->setOrigin(Origin::Left);
        value->setPosition((getSize().x * 0.67f), getSize().y / 2);
        value->setSizeMax(getSize().x * 0.5f, 0);
        add(value);

        sprite = new Sprite();
        add(sprite);
    }

    void update(Option *opt) {

        // always hide sprite (icon) first
        sprite->setVisibility(Visibility::Hidden);

        // reset
        name->setOutlineColor(textGroup.outlineColor);
        name->setStyle(Text::Regular);
        value->setVisibility(Visibility::Visible);

        option = opt;
        if (option) {
            name->setString(option->getName());
        } else {
            value->setString("GO");
            return;
        }

        if (option->getFlags() & Option::Flags::INPUT) {
            Skin::Button *button = ui->getSkin()->getButton(option->getValueInt());
            // don't use button textures on keyboard for now
            if (button && option->getId() < Option::Id::JOY_DEADZONE) {
                if (button->texture) {
                    sprite->setTexture(button->texture);
                    sprite->setVisibility(Visibility::Visible);
                    value->setVisibility(Visibility::Hidden);
                    float tex_scaling = std::min(
                            ((getSize().x * 0.33f) - 32) / sprite->getTextureRect().width,
                            (getSize().y / 2 + 4) / sprite->getTextureRect().height);
                    sprite->setScale(tex_scaling, tex_scaling);
                    sprite->setPosition((getSize().x * 0.66f) + 16, getSize().y / 2 - 3);
                    sprite->setOrigin(Origin::Left);
                } else {
                    value->setVisibility(Visibility::Visible);
                    value->setString(button->name);
                }
            } else {
                char btn[16];
                snprintf(btn, 16, "%i", option->getValueInt());
                value->setVisibility(Visibility::Visible);
                value->setString(btn);
            }
        } else if (option->getFlags() & Option::Flags::DELIMITER) {
            value->setVisibility(Visibility::Hidden);
            name->setStyle(Text::Underlined);
            name->setOutlineColor(ui->getUiMenu()->getOutlineColor());
        } else {
            value->setVisibility(Visibility::Visible);
            value->setString(option->getValueString());
        }
    }

    UIMain *ui = nullptr;
    Text *name = nullptr;
    Text *value = nullptr;
    Sprite *sprite = nullptr;
    Skin::TextGroup textGroup;
    Option *option;
};

UIMenu::UIMenu(UIMain *ui) : RectangleShape(Vector2f(0, 0)) {

    printf("GuiMenu (%p)\n", this);
    this->ui = ui;

    Skin *skin = ui->getSkin();

    skin->loadRectangleShape(this, {"OPTIONS_MENU"});
    alpha = getAlpha();

    // menu title
    title = new Text("TITLE", C2D_DEFAULT_CHAR_SIZE, ui->getSkin()->font);
    title->setStyle(Text::Underlined);
    skin->loadText(title, {"OPTIONS_MENU", "TITLE_TEXT"});
    add(title);

    textGroup = ui->getSkin()->getText({"OPTIONS_MENU", "ITEMS_TEXT"});
    int start_y = (int) textGroup.rect.top;
    // calculate lines per menu
    float line_height = ui->getSkin()->getFont()->getLineSpacing(textGroup.size) + 4;
    int max_lines = (int) ((getSize().y - start_y) / line_height) * 2;

    // add selection rectangle (highlight)
    highlight = new RectangleShape({16, 16});
    ui->getSkin()->loadRectangleShape(highlight, {"SKIN_CONFIG", "HIGHLIGHT"});
    highlight->setSize(((getSize().x / 2) * 0.5f) - 4, line_height);
    add(highlight);

    // add lines of text
    for (int i = 0; i < max_lines; i++) {
        FloatRect rect = {textGroup.rect.left, start_y + (i * line_height), getSize().x / 2, line_height};
        if (i >= max_lines / 2) {
#warning "todo: fix menu height on small screens"
            //rect.left = getSize().x / 2;
            //rect.top = start_y + ((i - ((float) max_lines / 2)) * line_height);
        }
        lines.push_back(new MenuLine(ui, rect, textGroup));
        add(lines[i]);
    }

    // build menus
    optionMenuGui = new OptionMenu(nullptr, ui->getConfig()->get());
    optionMenuGui->addChild("EXIT");
    optionMenuRom = new OptionMenu(nullptr, ui->getConfig()->get(true), true);
    optionMenuRom->addChild("STATES", true);
    optionMenuRom->addChild("RETURN");
    optionMenuRom->addChild("EXIT");

    tweenPosition = new TweenPosition({getPosition().x, -getSize().y}, getPosition(), 0.2f);
    tweenPosition->setState(TweenState::Stopped);
    add(tweenPosition);

    setVisibility(Visibility::Hidden);
}

void UIMenu::load(bool isRom, OptionMenu *om) {

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

    for (auto &line : lines) {
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

        lines[line_index]->update(option);
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

        lines[line_index]->update(nullptr);
        lines[line_index]->name->setString(optionMenu->childs[i]->title);
        lines[line_index]->setVisibility(Visibility::Visible);
        line_index++;
    }

    updateHighlight();

    if (!isVisible()) {
        setVisibility(Visibility::Visible, true);
        setLayer(1);
    }
}

void UIMenu::updateHighlight() {

    MenuLine *line = lines[optionIndex];
    highlight->setOrigin(Origin::Left);
    highlight->setPosition(line->getPosition() + line->value->getPosition());
    highlight->move(-4, 0);
    highlight->setSize(line->value->getLocalBounds().width + 8, highlight->getSize().y);
    for (size_t i = 0; i < lines.size(); i++) {
        Color color = (int) i == optionIndex ? getOutlineColor() : textGroup.outlineColor;
        lines.at(i)->value->setOutlineColor(color);
    }
}

bool UIMenu::onInput(c2d::Input::Player *players) {

    if (getUi()->getUiStateMenu()->isVisible()) {
        return C2DObject::onInput(players);
    }

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
            lines[optionIndex]->update(option);

            if (!option->getInfo().empty()) {
                ui->getUiMessageBox()->show("WARNING", option->getInfo(), "OK");
            }

            switch (option->getId()) {
                case Option::Id::GUI_SHOW_ALL:
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
                    lines[optionIndex]->update(option);
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

    return true;
}

bool UIMenu::isOptionHidden(Option *option) {
    return false;
}

bool UIMenu::isRom() {
    return isRomMenu;
}

UIMain *UIMenu::getUi() {
    return ui;
}

UIMenu::~UIMenu() {
    printf("~GuiMenu\n");
    delete (optionMenuGui);
    delete (optionMenuRom);
}

//
// Created by cpasjuste on 30/01/18.
//

#include "pemu.h"
#include "ui_menu.h"

using namespace c2d::config;

class MenuLine : public c2d::RectangleShape {

public:
    MenuLine(UiMain *u, FloatRect &rect, Skin::TextGroup &tg) : RectangleShape(rect) {
        pMain = u;
        m_textGroup = tg;
        Font *font = pMain->getSkin()->getFont();

        p_name = new Text("OPTION NAME", m_textGroup.size, font);
        p_name->setFillColor(m_textGroup.color);
        p_name->setOutlineThickness(m_textGroup.outlineSize);
        p_name->setOutlineColor(m_textGroup.outlineColor);
        p_name->setOrigin(Origin::Left);
        p_name->setPosition(2 * pMain->getScaling().x, MenuLine::getSize().y / 2);
        p_name->setSizeMax((MenuLine::getSize().x * 0.55f), 0);
        MenuLine::add(p_name);

        p_value = new Text("OPTION VALUE", m_textGroup.size, font);
        p_value->setFillColor(m_textGroup.color);
        p_value->setOutlineThickness(m_textGroup.outlineSize);
        p_value->setOutlineColor(m_textGroup.outlineColor);
        p_value->setOrigin(Origin::Left);
        p_value->setPosition((MenuLine::getSize().x * 0.6f), MenuLine::getSize().y / 2);
        p_value->setSizeMax(MenuLine::getSize().x * 0.38f, 0);
        MenuLine::add(p_value);

        p_sprite = new Sprite();
        MenuLine::add(p_sprite);
    }

    void refresh() {
        set(p_name->getString(), p_option);
    }

    void set(const std::string &name, Option *option) {
        p_option = option;

        // reset
        setVisibility(Visibility::Visible);
        p_sprite->setVisibility(Visibility::Hidden);
        p_name->setString(name);
        p_value->setVisibility(Visibility::Visible);
        setFillColor(Color::Transparent);

        // this is a menu title (or custom option)
        if (!option) {
            // custom options
            if (name == "STATES" || name == "QUIT") {
                p_value->setVisibility(Visibility::Visible);
                p_value->setString("GO");
                return;
            }
            p_value->setVisibility(Visibility::Hidden);
            setFillColor(pMain->getUiMenu()->getOutlineColor());
            return;
        }

        // this is an option
        if (option->getFlags() & PEMUConfig::Flags::INPUT) {
            Skin::Button *button = pMain->getSkin()->getButton(option->getInteger());
            if (button && option->getId() < PEMUConfig::OptId::JOY_DEADZONE) {
                if (button->texture) {
                    p_sprite->setTexture(button->texture, true);
                    p_sprite->setVisibility(Visibility::Visible);
                    p_value->setVisibility(Visibility::Hidden);
                    float scaling = std::min(
                            getSize().x / (float) p_sprite->getSize().x,
                            getSize().y / (float) p_sprite->getSize().y);
                    p_sprite->setScale(scaling, scaling);
                    p_sprite->setPosition((MenuLine::getSize().x * 0.6f), MenuLine::getSize().y / 2);
                    p_sprite->setOrigin(Origin::Left);
                } else {
                    p_sprite->setVisibility(Visibility::Hidden);
                    p_value->setVisibility(Visibility::Visible);
                    p_value->setString(button->name);
                }
            } else {
                char btn[16];
                snprintf(btn, 16, "%i", option->getInteger());
                p_value->setVisibility(Visibility::Visible);
                p_value->setString(btn);
            }
        } else {
            p_value->setVisibility(Visibility::Visible);
            p_value->setString(option->getString());
        }
    }

    UiMain *pMain = nullptr;
    Text *p_name = nullptr;
    Text *p_value = nullptr;
    Sprite *p_sprite = nullptr;
    Option *p_option = nullptr;
    Skin::TextGroup m_textGroup;
};

UiMenu::UiMenu(UiMain *uiMain) : SkinnedRectangle(uiMain, {"OPTIONS_MENU"}) {
    ui = uiMain;
    alpha = UiMenu::getAlpha();

    // menu title
    title = new SkinnedText(uiMain, {"OPTIONS_MENU", "TITLE_TEXT"});
    UiMenu::add(title);

    // retrieve skin config for options items
    textGroup = ui->getSkin()->getText({"OPTIONS_MENU", "ITEMS_TEXT"});

    // calculate number of items shown
    lineHeight = (float) textGroup.size + (2 * ui->getScaling().y);
    maxLines = (int) (UiMenu::getSize().y / lineHeight);
    if ((float) maxLines * lineHeight < UiMenu::getSize().y) {
        lineHeight = UiMenu::getSize().y / (float) maxLines;
    }

    // add selection rectangle (highlight)
    highlight = new RectangleShape({16, 16});
    ui->getSkin()->loadRectangleShape(highlight, {"SKIN_CONFIG", "HIGHLIGHT"});
    highlight->setSize(UiMenu::getSize().x - 2, lineHeight - (highlight->getOutlineThickness() * 2));
    highlight->move(1, 0);
    UiMenu::add(highlight);

    // add options items
    for (unsigned int i = 0; i < (unsigned int) maxLines; i++) {
        FloatRect r = {0, lineHeight * (float) i, UiMenu::getSize().x, lineHeight};
        auto line = new MenuLine(ui, r, textGroup);
        lines.push_back(line);
        UiMenu::add(line);
    }

    // tween
    Vector2f targetPos = {UiMenu::getPosition().x - UiMenu::getSize().x,
                          UiMenu::getPosition().y};
    tweenPosition = new TweenPosition({UiMenu::getPosition()}, targetPos, 0.2f);
    tweenPosition->setState(TweenState::Stopped);
    UiMenu::add(tweenPosition);

    // hide by default
    UiMenu::setVisibility(Visibility::Hidden);
}

void UiMenu::load(bool isGame) {
    isRomMenu = isGame;
    isEmuRunning = ui->getUiEmu()->isVisible();
    Game game = ui->getUiRomList()->getSelection();

    if (isRomMenu) {
        ui->getConfig()->loadGame(game);
        bool useZipName = ui->getConfig()->getOption(PEMUConfig::OptId::UI_SHOW_ZIP_NAMES)->getInteger();
        title->setString(useZipName ? Utility::removeExt(game.path) : game.name);
    } else {
        title->setString("MAIN OPTIONS");
    }

    // set options items
    optionIndex = highlightIndex = 0;
    menu_options.clear();
    auto groups = ui->getConfig()->getGroups();
    for (auto &group: *groups) {
        // skip some options if needed
        if (group.getName() == "ROMS") continue;
        if (isRomMenu && (group.getName() == "UI_OPTIONS" || group.getName() == "UI_FILTERING")) continue;
        // push group title
        menu_options.push_back({group.getName(), nullptr});
        auto options = group.getOptions();
        for (auto &option: *options) {
            // skip hidden options
            if (option.getFlags() & PEMUConfig::Flags::HIDDEN) continue;
            // push option
            auto opt = ui->getConfig()->get(option.getId(), isGame);
            menu_options.push_back({option.getName(), opt});
        }
    }

    // add "OTHER" option menu
    menu_options.push_back({"OTHER", nullptr});
    if (isRomMenu) menu_options.push_back({"STATES", nullptr});
    menu_options.push_back({"QUIT", nullptr});

    setAlpha(isEmuRunning ? (uint8_t) (alpha - 50) : (uint8_t) alpha);

    // update options lines/items (first option should always be a menu option)
    onKeyDown();

    // finally, show me
    if (!isVisible()) {
        setLayer(1);
        setVisibility(Visibility::Visible, true);
    }
}

void UiMenu::updateLines() {
    for (unsigned int i = 0; i < (unsigned int) maxLines; i++) {
        if (optionIndex + i >= menu_options.size()) {
            lines[i]->setVisibility(Visibility::Hidden);
            continue;
        }
        // set line data
        auto menuOption = menu_options.at(optionIndex + i);
        lines[i]->set(menuOption.name, menuOption.option);
        // set highlight position and color
        if ((int) i == highlightIndex) {
            highlight->setPosition({highlight->getPosition().x,
                                    lines[i]->getPosition().y + highlight->getOutlineThickness()});
            lines[i]->p_value->setOutlineColor(getOutlineColor());
        } else {
            lines[i]->p_value->setOutlineColor(textGroup.outlineColor);
        }
    }
}

void UiMenu::onKeyUp() {
    int index = optionIndex + highlightIndex;
    int middle = maxLines / 2;

    if (highlightIndex <= middle && index - middle > 0) {
        optionIndex--;
    } else {
        highlightIndex--;
    }

    if (highlightIndex < 0) {
        highlightIndex = (int) menu_options.size() < maxLines - 1 ? (int) menu_options.size() - 1 : maxLines - 1;
        optionIndex = ((int) menu_options.size() - 1) - highlightIndex;
    }

    // skip menus
    auto opt = menu_options.at(optionIndex + highlightIndex);
    if (opt.option == nullptr && opt.name != "STATES" && opt.name != "QUIT") {
        return onKeyUp();
    }

    updateLines();
}

void UiMenu::onKeyDown() {
    int index = optionIndex + highlightIndex;
    int middle = maxLines / 2;

    if (highlightIndex >= middle && index + (maxLines - middle) < (int) menu_options.size()) {
        optionIndex++;
    } else {
        highlightIndex++;
    }

    if (highlightIndex >= maxLines || optionIndex + highlightIndex >= (int) menu_options.size()) {
        optionIndex = 0;
        highlightIndex = 0;
    }

    // skip menus
    auto opt = menu_options.at(optionIndex + highlightIndex);
    if (opt.option == nullptr && opt.name != "STATES" && opt.name != "QUIT") {
        return onKeyDown();
    }

    updateLines();
}

bool UiMenu::onInput(c2d::Input::Player *players) {
    unsigned int buttons = players[0].buttons;

    if (ui->getUiStateMenu()->isVisible()) {
        return C2DObject::onInput(players);
    }

    // UP
    if (buttons & Input::Button::Up) {
        onKeyUp();
    }

    // DOWN
    if (buttons & Input::Button::Down) {
        onKeyDown();
    }

    // LEFT /RIGHT
    if (buttons & Input::Button::Left || buttons & Input::Button::Right) {
        auto option = lines.at(highlightIndex)->p_option;
        if (!option || option->getArray().size() <= 1) return true;
        needSave = true;
        if (buttons & Input::Button::Left) {
            option->setArrayMovePrev();
        } else {
            option->setArrayMoveNext();
        }
        if (!option->getComment().empty()) {
            ui->getUiStatusBox()->show(option->getComment());
        }

        switch (option->getId()) {
            case PEMUConfig::OptId::UI_FILTER_FAVORITES:
            case PEMUConfig::OptId::UI_FILTER_AVAILABLE:
            case PEMUConfig::OptId::UI_SHOW_ZIP_NAMES:
            case PEMUConfig::OptId::UI_FILTER_CLONES:
            case PEMUConfig::OptId::UI_FILTER_SYSTEM:
            case PEMUConfig::OptId::UI_FILTER_EDITOR:
            case PEMUConfig::OptId::UI_FILTER_DEVELOPER:
            case PEMUConfig::OptId::UI_FILTER_PLAYERS:
            case PEMUConfig::OptId::UI_FILTER_RATING:
            case PEMUConfig::OptId::UI_FILTER_DATE:
            case PEMUConfig::OptId::UI_FILTER_GENRE: {
                std::string name = Utility::toUpper(option->getName());
                std::string value = Utility::toUpper(option->getString());
                if (option->getComment().empty()) {
                    ui->getUiStatusBox()->show("%s: %s", name.c_str(), value.c_str());
                }
                ui->getUiRomList()->updateRomList();
                break;
            }

            case PEMUConfig::OptId::EMU_ROTATION:
            case PEMUConfig::OptId::EMU_SCALING:
                if (isEmuRunning) {
                    ui->getUiEmu()->getVideo()->updateScaling();
                    auto gw = (float) ui->getUiEmu()->getVideo()->getTextureRect().width;
                    auto gh = (float) ui->getUiEmu()->getVideo()->getTextureRect().height;
                    float gr = std::max(
                            (float) ui->getUiEmu()->getVideo()->aspect.x /
                            (float) ui->getUiEmu()->getVideo()->aspect.y,
                            (float) ui->getUiEmu()->getVideo()->aspect.y /
                            (float) ui->getUiEmu()->getVideo()->aspect.x);
                    float ow = gw * ui->getUiEmu()->getVideo()->getScale().x;
                    float oh = gh * ui->getUiEmu()->getVideo()->getScale().y;
                    float ratio = std::max(ow / oh, oh / ow);
                    ui->getUiStatusBox()->show(
                            "GAME: %ix%i - RATIO: %.2f | OUTPUT: %ix%i - RATIO: %.2f - SCALING: %.2fx%.2f",
                            (int) gw, (int) gh, gr, (int) ow, (int) oh, ratio,
                            ui->getUiEmu()->getVideo()->getScale().x, ui->getUiEmu()->getVideo()->getScale().y);
                }
                break;
            case PEMUConfig::OptId::EMU_SCALING_MODE:
                if (option->getString() == "AUTO") {
                    ui->getUiStatusBox()->show("TRY TO KEEP INTEGER SCALING IF ASPECT RATIO IS NOT TOO DIVERGENT");
                } else if (option->getString() == "ASPECT") {
                    ui->getUiStatusBox()->show("KEEP GAME ASPECT RATIO - SOME SHADERS MAY NOT RENDER CORRECTLY");
                } else {
                    ui->getUiStatusBox()->show(
                            "FORCE INTEGER SCALING - ASPECT RATIO MAY BE WRONG BUT SHADERS WILL RENDER CORRECTLY");
                }
                if (isEmuRunning) {
                    ui->getUiEmu()->getVideo()->updateScaling();
                }
                break;
            case PEMUConfig::OptId::EMU_FILTER:
                if (isEmuRunning) {
                    ui->getUiEmu()->getVideo()->setFilter((Texture::Filter) option->getArrayIndex());
                }
                break;
            case PEMUConfig::OptId::EMU_SHADER:
                if (isEmuRunning) {
                    ui->getUiEmu()->getVideo()->setShader(option->getArrayIndex());
                    ui->getUiStatusBox()->show(option->getString());
                }
                break;
#ifdef __VITA__
                case PEMUConfig::OptId::EMU_WAIT_RENDERING:
                        if (isEmuRunning) {
                            ((PSP2Renderer *) ui)->setWaitRendering(option->getInteger());
                        }
                        break;
#endif
            case PEMUConfig::OptId::UI_VIDEO_SNAP_DELAY:
                ui->getUiRomList()->setVideoSnapDelay(option->getInteger());
                break;

            default:
                break;
        }

        // update option line
        lines.at(highlightIndex)->refresh();
    }

    // FIRE1 (ENTER)
    if (buttons & Input::Button::A) {
        auto option = lines.at(highlightIndex)->p_option;
        if (option && option->getFlags() == PEMUConfig::Flags::INPUT) {
            int new_key = 0;
            int res = ui->getUiMessageBox()->show("NEW INPUT", "PRESS A BUTTON", "", "", &new_key, 9);
            if (res != MessageBox::TIMEOUT) {
                needSave = true;
                option->setInteger(new_key);
                lines.at(highlightIndex)->refresh();
            }
        } else if (lines.at(highlightIndex)->p_name->getString() == "STATES") {
            setVisibility(Visibility::Hidden, true);
            ui->getUiStateMenu()->setVisibility(Visibility::Visible, true);
        } else if (lines.at(highlightIndex)->p_name->getString() == "QUIT") {
            if (isEmuRunning) {
                setVisibility(Visibility::Hidden, true);
                ui->getUiEmu()->stop();
                ui->getUiRomList()->setVisibility(Visibility::Visible);
                ui->getInput()->clear();
            } else {
                // be sure options are saved before exiting
                if (isRomMenu) ui->getConfig()->saveGame();
                else ui->getConfig()->save();
                needSave = false;
                ui->done = true;
            }
        }
    }

    // FIRE2 (BACK)
    if (buttons & Input::Button::Menu1 || buttons & Input::Button::Menu2 || buttons & Input::Button::B) {
        setVisibility(Visibility::Hidden, true);
        if (isEmuRunning) {
            ui->getUiEmu()->resume();
        }
    }

    return true;
}

void UiMenu::setVisibility(Visibility visibility, bool tweenPlay) {
    if (ui->getUiRomList() && ui->getUiRomList()->isVisible()) {
        ui->getUiRomList()->getBlur()->setVisibility(visibility, true);
    }

    if (visibility == Visibility::Hidden && needSave) {
        if (isRomMenu) ui->getConfig()->saveGame();
        else ui->getConfig()->save();
        needSave = false;
    }

    RectangleShape::setVisibility(visibility, tweenPlay);
}

UiMenu::~UiMenu() {
    printf("~UIMenuNew\n");
}

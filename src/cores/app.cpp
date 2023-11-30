//
// Created by cpasjuste on 08/12/2021.
//

#include "app.h"

using namespace c2d;
using namespace RRLauncher;

App::App(const c2d::Vector2f &size) : Skin(size) {
    // custom io
    auto io = new PEMUIo();
    App::setIo(io);

    // load configuration
    int pemu_version = (__PEMU_VERSION_MAJOR__ * 100) + __PEMU_VERSION_MINOR__;
    mConfig = new PEMUConfig(io, pemu_version);

    // build roms list
    mRomList = new PEMURomList(mConfig, mConfig->getCoreVersion(), mConfig->getCoreSupportedExt());
    mRomList->build();
    //mRomList->initFav(); // TODO

    App::getInput()->setRepeatDelay(300);
    App::setName("app");
    App::load();
}

std::string App::getVar(const std::string &var, const std::string &data, int index) {
    //rr_debug("App::getVar(%s, %s, %i)\n", var.c_str(), data.c_str(), index);
    if (!data.empty()) {
        // we want a list data
        if (data == "games" && mRomList) {
            size_t i = index < 0 ? mRomListIndex : (size_t) index;
            if (i < mRomList->gameList->games.size()) {
                Game game = mRomList->gameList->games.at(i);
                if (var == "NAME") {
                    return game.name;
                } else if (var == "DESCRIPTION") {
                    return game.synopsis;
                } else if (var == "EDITOR") {
                    return game.editor.name;
                } else if (var == "DEVELOPER") {
                    return game.developer.name;
                } else if (var == "GENRE") {
                    return game.genre.name;
                } else if (var == "RATING") {
                    return std::to_string(game.rating);
                } else if (var == "PLAYERS") {
                    return game.players;
                } else if (var == "AVAILABLE") {
                    return game.available ? "yes" : "no";
                } else if (var == "CLONEOF") {
                    return game.cloneOf;
                } else if (var == "DATE") {
                    return game.date;
                } else if (var == "FILENAME") {
                    return game.path;
                } else if (var == "PATH") {
                    return game.romsPath;
                } else if (var == "TEXTURE") {
                    return game.romsPath + game.getMedia("mixrbv2").url;
                }
            } else {
                rr_error("var: %s, data: %s, index: %zu is greater than games count (%zu)",
                         var.c_str(), data.c_str(), i, mRomList->gameList->games.size());
            }
        } else if (data == "main_options") {
            size_t i = index < 0 ? mOptionIndex : (size_t) index;
            if (i < mConfig->getConfigItems(false)->size()) {
                auto item = mConfig->getConfigItems(false)->at(i);
                if (var == "NAME") {
                    if (item.group) {
                        std::string s = item.group->getName();
                        std::replace(s.begin(), s.end(), '_', ' ');
                        return s;
                    }
                    else return item.option->getName();
                } else if (var == "VALUE") {
                    if (item.option) return item.option->getString();
                    else return {};
                }
            } else {
                rr_error("var: %s, data: %s, index: %zu is greater than main_options count (%zu)",
                         var.c_str(), data.c_str(), i, mConfig->getConfigItems(false)->size());
            }
        }
    } else if (var == "APP_NAME") {
        return mConfig ? mConfig->getName() : "app";
    } else if (var == "APP_VERSION") {
        return mConfig ? mConfig->getCoreVersion() : "1.0.0";
    }

    return data + "[" + std::to_string(index) + "]." + var + ": NOK";
}

size_t App::getVarInt(const std::string &var, const std::string &data, int index) {
    //rr_debug("App::getVarInt(%s, %s, %i)\n", var.c_str(), data.c_str(), index);
    if (!data.empty()) {
        // we want a list data
        if (data == "games") {
        }
    }

    return 0;
}

void App::updateData(const std::string &name) {
    //rr_debug("App::updateData(%s)\n", name.c_str());
    if (name == "games") {
        // TODO
    }
}

size_t App::getDataCount(const std::string &name) {
    //rr_debug("App::getDataCount(%s)\n", name.c_str());
    if (name == "games") {
        return mRomList ? mRomList->gameList->games.size() : 0;
    } else if (name == "main_options") {
        return mConfig->getConfigItems(false)->size();
    }

    return 0;
}

void App::setDataIndex(const std::string &name, size_t index) {
    //rr_debug("App::setDataIndex(%s, %zu)\n", name.c_str(), index);
    if (name == "games") {
        if (mRomList && index < mRomList->gameList->games.size()) {
            mRomListIndex = index;
            rr_debug("%s: index set to %zu", name.c_str(), index);
        }
    } else if (name == "main_options") {
        if (mConfig && index < mConfig->getConfigItems(false)->size()) {
            mOptionIndex = index;
            rr_debug("%s: index set to %zu", name.c_str(), index);
        }
    }
}

void App::setDataFilter(const std::string &name, const std::string &filter) {
    if (name == "games") {
        // TODO
    }
}

// onUpdate is called every frames
void App::onUpdate() {
    C2DRenderer::onUpdate();
}

// onInput is only called when a key is pressed
bool App::onInput(c2d::Input::Player *players) {
    unsigned int buttons = players[0].buttons;

    // quit app (enter and space on a keyboard)
    if (buttons & Input::Button::Quit) {
        quit = true;
    } else if (buttons & Input::Button::Select) {
        // m_pacman->install({"powermanga", "opentyrian", "rawgl"});
    }

    return C2DRenderer::onInput(players);
}

App::~App() {
    delete (mRomList);
    delete (mConfig);
}

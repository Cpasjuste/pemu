//
// Created by cpasjuste on 13/09/23.
//

#ifndef C2DUI_CONFIG_NEW_H
#define C2DUI_CONFIG_NEW_H

namespace c2dui {
    class ConfigNew : public c2d::config::Config {
    public:
        ConfigNew(c2d::Io *io, const std::string &name, int version = 1);

        ~ConfigNew();

        bool loadGame(const Game &game);

        bool saveGame();

        c2d::config::Option *getOption(int id, bool isGame = false);

        std::vector<c2d::Input::ButtonMapping> getKeyboardMapping(int player, bool isGame = false);

        std::vector<c2d::Input::ButtonMapping> getJoystickMapping(int player, bool isGame = false);

        c2d::Vector2i getJoystickAxisLeftMapping(int player, bool isGame = false);

        c2d::Vector2i getJoystickAxisRightMapping(int player, bool isGame = false);

        int getJoystickDeadZone(int player, bool isGame = false);

    private:
        c2d::config::Config *p_game_config = nullptr;
        c2d::Io *p_io = nullptr;
    };
}

#endif //C2DUI_CONFIG_NEW_H

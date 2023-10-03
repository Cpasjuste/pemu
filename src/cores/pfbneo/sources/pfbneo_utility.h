//
// Created by cpasjuste on 28/09/23.
//

#ifndef PEMU_PFBNEO_UTILITY_H
#define PEMU_PFBNEO_UTILITY_H

#include <string>

#define HARDWARE_PREFIX_ARCADE 0x12341234

class PFBNEOUtility {
public:
    struct GameInfo {
        std::string name;
        std::string manufacturer;
        std::string parent;
        std::string sysName;
        int sysId;
        int players;
    };

    static int setDriverActive(const Game &game);

    static GameInfo getGameInfo(const Game &game);
};

#endif //PEMU_PFBNEO_UTILITY_H

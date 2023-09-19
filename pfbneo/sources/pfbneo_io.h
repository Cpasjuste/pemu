//
// Created by cpasjuste on 27/06/19.
//

#ifndef PEMU_PFBNEO_IO_H
#define PEMU_PFBNEO_IO_H

namespace c2d {

    class PFBAIo : public c2d::C2DIo {

    public:
#ifdef __PSP2__
        std::string getDataPath() override {
            return "ux0:/data/pfba/";
        }
#elif __PS4__
        std::string getDataPath() override {
            return "/data/pfba/";
        }
#ifndef NDEBUG
        std::string getRomFsPath() override {
            return "/data/pfba/";
        }

#endif
#elif __3DS__
#ifndef NDEBUG
        std::string getDataPath() override {
            return "/3ds/pfbn/";
        }
#endif
#elif __SWITCH__
#if __DEBUG_SVC__
        std::string getDataPath() override {
            return "/switch/pfbn/";
        }
#endif
#endif
    };
}

#endif //PEMU_PFBNEO_IO_H

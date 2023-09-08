//
// Created by cpasjuste on 03/04/2020.
//

#ifndef PGEN_IO_H
#define PGEN_IO_H

namespace c2d {

    class PGENIo : public c2d::C2DIo {

    public:
#ifdef __PSP2__
        std::string getDataPath() override {
            return "ux0:/data/pgen/";
        }
#elif __PS4__
        std::string getDataPath() override {
            return "/data/pgen/";
        }
#ifndef NDEBUG
        std::string getRomFsPath() override {
            return "/data/pgen/";
        }
#endif
#elif __3DS__
#ifndef NDEBUG
        std::string getDataPath() override {
            return "/3ds/pgen/";
        }
#endif
#elif __SWITCH__
#if __DEBUG_SVC__
        std::string getDataPath() override {
            return "/switch/pgen/";
        }
#endif
#endif
    };
}

#endif //PGEN_IO_H

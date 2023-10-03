//
// Created by cpasjuste on 03/04/2020.
//

#ifndef PGBA_IO_H
#define PGBA_IO_H

namespace c2d {
    class PGBAIo : public c2d::C2DIo {
    public:
        PGBAIo() : C2DIo() {
            C2DIo::create(PGBAIo::getDataPath());
            C2DIo::create(PGBAIo::getDataPath() + "bios");
            C2DIo::create(PGBAIo::getDataPath() + "configs");
            C2DIo::create(PGBAIo::getDataPath() + "saves");
            C2DIo::create(PGBAIo::getDataPath() + "roms");
            C2DIo::create(PGBAIo::getDataPath() + "patches");
            C2DIo::create(PGBAIo::getDataPath() + "cheats");
        }

#ifdef __PSP2__
        std::string getDataPath() override {
            return "ux0:/data/pgba/";
        }
#elif __PS4__
        std::string getDataPath() override {
            return "/data/pgba/";
        }
#ifndef NDEBUG
        std::string getRomFsPath() override {
            return "/data/pgba/";
        }
#endif
#elif __3DS__
#ifndef NDEBUG
        std::string getDataPath() override {
            return "/3ds/pgba/";
        }
#endif
#elif __SWITCH__
#if __DEBUG_SVC__

        std::string getDataPath() override {
            return "/switch/pgba/";
        }

#endif
#endif
    };
}

#endif //PGBA_IO_H

//
// Created by cpasjuste on 03/04/2020.
//

#ifndef PEMU_PNES_IO_H
#define PEMU_PNES_IO_H

namespace c2d {

    class PNESIo : public c2d::C2DIo {
    public:
        PNESIo() : C2DIo() {
            C2DIo::create(PNESIo::getDataPath());
            C2DIo::create(PNESIo::getDataPath() + "roms");
            C2DIo::create(PNESIo::getDataPath() + "configs");
            C2DIo::create(PNESIo::getDataPath() + "saves");
        }

#ifdef __PSP2__
        std::string getDataPath() override {
            return "ux0:/data/pnes/";
        }
#elif __PS4__
        std::string getDataPath() override {
            return "/data/pnes/";
        }
#ifndef NDEBUG
        std::string getRomFsPath() override {
            return "/data/pnes/";
        }
#endif
#elif __3DS__
#ifndef NDEBUG
        std::string getDataPath() override {
            return "/3ds/pnes/";
        }
#endif
#elif __SWITCH__
#if __DEBUG_SVC__
        std::string getDataPath() override {
            return "/switch/pnes/";
        }
#endif
#endif
    };
}

#endif //PEMU_PNES_IO_H

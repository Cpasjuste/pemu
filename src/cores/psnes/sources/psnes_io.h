//
// Created by cpasjuste on 03/04/2020.
//

#ifndef PEMU_PSNES_IO_H
#define PEMU_PSNES_IO_H

namespace c2d {
    class PSNESIo : public c2d::C2DIo {
    public:
        PSNESIo() : C2DIo() {
            C2DIo::create(C2DIo::getDataPath());
            C2DIo::create(C2DIo::getDataPath() + "roms");
            C2DIo::create(C2DIo::getDataPath() + "configs");
            C2DIo::create(C2DIo::getDataPath() + "saves");
        }

#ifdef __PSP2__
        std::string getDataPath() override {
            return "ux0:/data/psnes/";
        }
#elif __PS4__
        std::string getDataPath() override {
            return "/data/psnes/";
        }
#elif __3DS__
#ifndef NDEBUG
        std::string getDataPath() override {
            return "/3ds/psnes/";
        }
#endif
#elif __SWITCH__
#if __DEBUG_SVC__
        std::string getDataPath() override {
            return "/switch/psnes/";
        }
#endif
#endif
    };
}

#endif //PEMU_PSNES_IO_H

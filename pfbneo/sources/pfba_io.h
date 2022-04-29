//
// Created by cpasjuste on 27/06/19.
//

#ifndef PEMU_PFBA_IO_H
#define PEMU_PFBA_IO_H

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
#endif
    };
}

#endif //PEMU_PFBA_IO_H

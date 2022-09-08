//
// Created by cpasjuste on 03/04/2020.
//

#ifndef PGBA_IO_H
#define PGBA_IO_H

namespace c2d {

    class PGBAIo : public c2d::C2DIo {

    public:
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
#endif
    };
}

#endif //PGBA_IO_H

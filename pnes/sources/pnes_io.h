//
// Created by cpasjuste on 03/04/2020.
//

#ifndef PEMU_PNESIO_H
#define PEMU_PNESIO_H

namespace c2d {

    class PNESIo : public c2d::C2DIo {

    public:

#ifdef __PSP2__
        std::string getHomePath() override {
            return "ux0:/data/pnes/";
        }

        std::string getDataPath() override {
            return "ux0:/data/pnes/";
        }
#elif __PS4__

        std::string getHomePath() override {
            return "/data/pnes/";
        }

        std::string getDataPath() override {
            return "/data/pnes/";
        }

#ifndef NDEBUG

        std::string getRomFsPath() override {
            return "/data/pnes/";
        }

#endif

#endif

    };
}

#endif //PEMU_PNESIO_H

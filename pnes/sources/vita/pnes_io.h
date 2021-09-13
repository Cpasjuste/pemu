//
// Created by cpasjuste on 27/06/19.
//

#ifndef PEMU_PNES_IO_H
#define PEMU_PNES_IO_H

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
#endif
    };
}

#endif //PEMU_PNES_IO_H

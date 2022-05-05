//
// Created by cpasjuste on 03/04/2020.
//

#ifndef PEMU_PSNESIO_H
#define PEMU_PSNESIO_H

namespace c2d {

    class PSNESIo : public c2d::C2DIo {

    public:
#ifdef __PSP2__
        std::string getDataPath() override {
            return "ux0:/data/psnes/";
        }
#elif __PS4__
        std::string getDataPath() override {
            return "/data/psnes/";
        }
#endif
    };
}

#endif //PEMU_PSNESIO_H

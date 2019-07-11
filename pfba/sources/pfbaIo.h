//
// Created by cpasjuste on 27/06/19.
//

#ifndef PEMU_PFBAIO_H
#define PEMU_PFBAIO_H

namespace c2d {

    class PFBAIo : public c2d::C2DIo {

    public:

#ifdef __PSP2__

        std::string getHomePath() const override {
            return "ux0:/data/pfba/";
        }

        std::string getDataWritePath() const override {
            return "ux0:/data/pfba/";
        }

#elif __FREEPLAY__

        std::string getHomePath() const override {
            return "/data/pfba/";
        }

        std::string getDataWritePath() const override {
            return "/data/pfba/";
        }

#endif

    };
}

#endif //PEMU_PFBAIO_H

//
// Created by cpasjuste on 27/06/18.
//

#ifndef PSNES_UNZIP_ROM_H
#define PSNES_UNZIP_ROM_H

class Unzip {

public:

    static bool extract(const std::string &zipFile, const std::string &dstFile);
};


#endif //PSNES_UNZIP_ROM_H

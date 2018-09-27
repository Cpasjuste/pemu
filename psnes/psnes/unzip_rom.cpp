//
// Created by cpasjuste on 27/06/18.
//

#include <string>
#include <cstring>

#include "unzip_rom.h"
#include "unzip/unzip.h"

bool Unzip::extract(const std::string &zipFile, const std::string &dstFile) {

    unzFile zip = unzOpen(zipFile.c_str());

    if (zip == nullptr) {
        return false;
    }

    if (unzGoToFirstFile(zip) != UNZ_OK) {
        unzClose(zip);
        return false;
    }

    if (unzOpenCurrentFile(zip) != UNZ_OK) {
        unzClose(zip);
        return false;
    }

    char szName[256] = "";
    unz_file_info info;
    memset(&info, 0, sizeof(info));

    if (unzGetCurrentFileInfo(zip, &info, szName, 256, NULL, 0, NULL, 0) != UNZ_OK) {
        unzCloseCurrentFile(zip);
        unzClose(zip);
        return false;
    }

    if (info.uncompressed_size > 0x800000 + 512) {
        unzCloseCurrentFile(zip);
        unzClose(zip);
        return false;
    }

    auto *buffer = (unsigned char *) malloc(info.uncompressed_size);
    if (!buffer) {
        unzCloseCurrentFile(zip);
        unzClose(zip);
        return false;
    }

    int len = unzReadCurrentFile(zip, buffer, (unsigned int) info.uncompressed_size);
    if (len < 0) {
        free(buffer);
        unzCloseCurrentFile(zip);
        unzClose(zip);
        return false;
    }

    unzCloseCurrentFile(zip);
    unzClose(zip);

    FILE *fp;
    fp = fopen(dstFile.c_str(), "wb");
    if (!fp) {
        free(buffer);
        return false;
    }

    fwrite(buffer, sizeof(unsigned char), (size_t) len, fp);
    fclose(fp);
    free(buffer);

    return true;
}

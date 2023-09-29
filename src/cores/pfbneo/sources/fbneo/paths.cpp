// Paths module
#include "cross2d/c2d.h"
#include "burner.h"

/////////
// FBA
/////////
#include "version.h"

#define s(A) #A
#define a(A) s(A)
#define VERSION a(VER_MAJOR.VER_MINOR.VER_BETA.VER_ALPHA)
char szAppBurnVer[16] = VERSION;

// windows fix
#undef MAX_PATH
#define MAX_PATH 512

// pfbneo
char szAppHomePath[MAX_PATH];
char szAppRomPath[MAX_PATH];
char szAppSavePath[MAX_PATH];
char szAppConfigPath[MAX_PATH];
char szAppIconPath[MAX_PATH];
// fbneo
char szAppHiscorePath[MAX_PATH];
char szAppSamplesPath[MAX_PATH];
char szAppBlendPath[MAX_PATH];
char szAppEEPROMPath[MAX_PATH];
char szAppHDDPath[MAX_PATH];

void BurnPathsInit(c2d::C2DIo *io) {
    printf("BurnPathsInit: dataPath = %s\n", io->getDataPath().c_str());

    snprintf(szAppHomePath, MAX_PATH - 1, "%s", io->getDataPath().c_str());
    io->create(szAppHomePath);

    snprintf(szAppRomPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "arcade");
    io->create(szAppRomPath);

    snprintf(szAppSavePath, MAX_PATH - 1, "%s%s/", szAppHomePath, "saves");
    io->create(szAppSavePath);
    //printf("szAppSavePath: %s\n", szAppSavePath);

    snprintf(szAppConfigPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "configs");
    io->create(szAppConfigPath);
    //printf("szAppConfigPath: %s\n", szAppConfigPath);

    snprintf(szAppSamplesPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "samples");
    io->create(szAppSamplesPath);
    //printf("szAppSamplesPath: %s\n", szAppSamplesPath);

    snprintf(szAppIconPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "icons");
    io->create(szAppIconPath);
    //printf("szAppIconPath: %s\n", szAppIconPath);

    snprintf(szAppBlendPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "blend");
    io->create(szAppBlendPath);
    //printf("szAppBlendPath: %s\n", szAppBlendPath);

    snprintf(szAppHDDPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "hdd");
    io->create(szAppHDDPath);
    //printf("szAppHDDPath: %s\n", szAppHDDPath);

    snprintf(szAppEEPROMPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "eeproms");
    io->create(szAppEEPROMPath);

    snprintf(szAppHiscorePath, MAX_PATH - 1, "%s%s/", szAppHomePath, "hiscores");
    // copy hiscore.dat from romfs to datadir
    io->copy(io->getRomFsPath() + "hiscores/", std::string(szAppHiscorePath) + "/");
}


// Paths module
#include <unistd.h>
#include <sys/stat.h>
#include "burner.h"

/////////
// FBA
/////////
char szAppBurnVer[16] = VERSION;
// replaces ips_manager.cpp
bool bDoIpsPatch = 0;

void IpsApplyPatches(UINT8 *base, char *rom_name) {}

// needed by cps3run.cpp and dataeast/d_backfire.cpp
void Reinitialise() {}

// needed by neo_run.cpp
void wav_exit() {}

int bRunPause;
/////////
// FBA
/////////

#ifdef __PSP2__
#include <psp2/io/stat.h>
#define mkdir sceIoMkdir
#elif __WINDOWS__
#define mkdir(x, y) mkdir(x)
#endif

char szAppHomePath[MAX_PATH];
char szAppRomPath[MAX_PATH];
char szAppSavePath[MAX_PATH];
char szAppConfigPath[MAX_PATH];
char szAppHiscorePath[MAX_PATH];
char szAppSamplesPath[MAX_PATH];
char szAppIconPath[MAX_PATH];
char szAppBlendPath[MAX_PATH];
char szAppEEPROMPath[MAX_PATH];
char szAppHDDPath[MAX_PATH];
char szAppSkinPath[MAX_PATH];

void BurnPathsInit(const char *dataPath) {

    printf("BurnPathsInit: dataPath = %s\n", dataPath);

    snprintf(szAppHomePath, MAX_PATH - 1, "%s/", dataPath);
    mkdir(szAppHomePath, 0777);

    snprintf(szAppRomPath, MAX_PATH - 1, "%s%s", szAppHomePath, "roms");
    mkdir(szAppRomPath, 0777);

    snprintf(szAppSavePath, MAX_PATH - 1, "%s%s", szAppHomePath, "saves");
    mkdir(szAppSavePath, 0777);
    //printf("szAppSavePath: %s\n", szAppSavePath);

    snprintf(szAppConfigPath, MAX_PATH - 1, "%s%s", szAppHomePath, "configs");
    mkdir(szAppConfigPath, 0777);
    //printf("szAppConfigPath: %s\n", szAppConfigPath);

    snprintf(szAppSamplesPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "samples");
    mkdir(szAppSamplesPath, 0777);
    //printf("szAppSamplesPath: %s\n", szAppSamplesPath);

    snprintf(szAppIconPath, MAX_PATH - 1, "%s%s", szAppHomePath, "icons");
    mkdir(szAppIconPath, 0777);
    //printf("szAppIconPath: %s\n", szAppIconPath);

    snprintf(szAppBlendPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "blend");
    mkdir(szAppBlendPath, 0777);
    //printf("szAppBlendPath: %s\n", szAppBlendPath);

    snprintf(szAppHDDPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "hdd");
    mkdir(szAppHDDPath, 0777);
    //printf("szAppHDDPath: %s\n", szAppHDDPath);

    snprintf(szAppEEPROMPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "eeproms");
    mkdir(szAppEEPROMPath, 0777);

    snprintf(szAppHiscorePath, MAX_PATH - 1, "%s%s/", szAppHomePath, "hiscores");
    mkdir(szAppHiscorePath, 0777);
    //printf("szAppHiscorePath: %s\n", szAppHiscorePath);
}

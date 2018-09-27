
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
#endif

char szAppHomePath[MAX_PATH];
char szAppRomPath[MAX_PATH];
char szAppSavePath[MAX_PATH];
char szAppConfigPath[MAX_PATH];
char szAppHiscorePath[MAX_PATH];
char szAppSamplesPath[MAX_PATH];
char szAppPreviewPath[MAX_PATH];
char szAppTitlePath[MAX_PATH];
char szAppIconPath[MAX_PATH];
char szAppBlendPath[MAX_PATH];
char szAppEEPROMPath[MAX_PATH];
char szAppSkinPath[MAX_PATH];

void BurnPathsInit() {
#ifdef __PSP2__
    strncpy(szAppHomePath, "ux0:/data/pfba/", MAX_PATH);
    strncpy(szAppSkinPath, "app0:/skin/", MAX_PATH);
#elif __3DS__
    strncpy(szAppHomePath, "/pfba", MAX_PATH);
#elif __PS3__
    strncpy(szAppHomePath, "/dev_hdd0/pfba", MAX_PATH);
#elif __SWITCH__
    strncpy(szAppHomePath, "./", MAX_PATH);
#else
    getcwd(szAppHomePath, MAX_PATH);
    strcat(szAppHomePath, "/");
    mkdir(szAppHomePath, 0777);
#endif
    //printf("szAppHomePath: %s\n", szAppHomePath);

    snprintf(szAppRomPath, MAX_PATH - 1, "%s%s", szAppHomePath, "roms");
    mkdir(szAppRomPath, 0777);

    snprintf(szAppSavePath, MAX_PATH - 1, "%s%s", szAppHomePath, "saves");
    mkdir(szAppSavePath, 0777);
    //printf("szAppSavePath: %s\n", szAppSavePath);

    snprintf(szAppConfigPath, MAX_PATH - 1, "%s%s", szAppHomePath, "configs");
    mkdir(szAppConfigPath, 0777);
    //printf("szAppConfigPath: %s\n", szAppConfigPath);

    snprintf(szAppHiscorePath, MAX_PATH - 1, "%s%s/", szAppHomePath, "hiscores");
    mkdir(szAppHiscorePath, 0777);
    //printf("szAppHiscorePath: %s\n", szAppHiscorePath);

    snprintf(szAppSamplesPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "samples");
    mkdir(szAppSamplesPath, 0777);
    //printf("szAppSamplesPath: %s\n", szAppSamplesPath);

    snprintf(szAppPreviewPath, MAX_PATH - 1, "%s%s", szAppHomePath, "previews");
    mkdir(szAppPreviewPath, 0777);
    //printf("szAppPreviewPath: %s\n", szAppPreviewPath);

    snprintf(szAppTitlePath, MAX_PATH - 1, "%s%s", szAppHomePath, "titles");
    mkdir(szAppTitlePath, 0777);
    //printf("szAppTitlePath: %s\n", szAppTitlePath);

    snprintf(szAppIconPath, MAX_PATH - 1, "%s%s", szAppHomePath, "icons");
    mkdir(szAppIconPath, 0777);
    //printf("szAppIconPath: %s\n", szAppIconPath);

    snprintf(szAppBlendPath, MAX_PATH - 1, "%s%s/", szAppHomePath, "blend");
    mkdir(szAppBlendPath, 0777);
    //printf("szAppBlendPath: %s\n", szAppBlendPath);

#ifndef __PSP2__
    snprintf(szAppSkinPath, MAX_PATH - 1, "%s%s", szAppHomePath, "skin");
    mkdir(szAppSkinPath, 0777);
    //printf("szAppSkinPath: %s\n", szAppSkinPath);
#endif

    snprintf(szAppEEPROMPath, MAX_PATH - 1, "%sconfig", szAppHomePath);
    mkdir(szAppEEPROMPath, 0777);
    strncat(szAppEEPROMPath, "/games/", MAX_PATH - 1);
    mkdir(szAppEEPROMPath, 0777);
}

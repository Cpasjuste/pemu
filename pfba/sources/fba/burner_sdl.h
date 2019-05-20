/*
 * FinalBurn Alpha for Dingux/OpenDingux
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _BURNER_H_
#define _BURNER_H_

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <malloc.h>

#ifdef __PSP2_DEBUG__
#include <psp2/kernel/clib.h>
#define printf sceClibPrintf
#endif

#ifndef MAX_PATH
#define MAX_PATH 511
#endif

// Macro to make quoted strings
#define MAKE_STRING_2(s) #s
#define MAKE_STRING(s) MAKE_STRING_2(s)

#define BZIP_MAX (20)                                // Maximum zip files to search through
#undef DIRS_MAX
#define DIRS_MAX (20)                                // Maximum number of directories to search

// Define macros for appliction title and description
#ifdef FBA_DEBUG
#define APP_TITLE "FB Alpha [DEBUG]"
#else
#define APP_TITLE "FB Alpha"
#endif

#define APP_DESCRIPTION "Emulator for arcade games"

#include "tchar.h"
#include "version.h"
//#include "burn.h"

// ---------------------------------------------------------------------------
// FIXME: this is needed by interface.h (eliminate later)
#ifndef RECT
typedef struct tagRECT {
    int left;
    int top;
    int right;
    int bottom;
} RECT, *PRECT, *LPRECT;
typedef const RECT *LPCRECT;
#endif

#include "interface.h"
// ---------------------------------------------------------------------------
// OS independent functionality

#define _s(A) #A
#define _a(A) _s(A)
#define VERSION _a(VER_MAJOR.VER_MINOR.VER_BETA.VER_ALPHA)


#define IMG_FREE        (1 << 0)

// Macros for parsing text
#define SKIP_WS(s) while (_istspace(*s)) { s++; }            // Skip whitespace
#define FIND_WS(s) while (*s && !_istspace(*s)) { s++; }    // Find whitespace
#define FIND_QT(s) while (*s && *s != _T('\"')) { s++; }    // Find quote

// dat.cpp
#define DAT_ARCADE_ONLY        0
#define DAT_MEGADRIVE_ONLY    1
#define DAT_PCENGINE_ONLY    2
#define DAT_TG16_ONLY        3
#define DAT_SGX_ONLY        4

char *DecorateGameName(UINT32 nBurnDrv);

INT32 write_datfile(INT32 bType, FILE *fDat);

INT32 create_datfile(TCHAR *szFilename, INT32 bType);

// state.cpp
INT32 BurnStateLoadEmbed(FILE *fp, INT32 nOffset, INT32 bAll, INT32 (*pLoadGame)());

INT32 BurnStateLoad(TCHAR *szName, INT32 bAll, INT32 (*pLoadGame)());

INT32 BurnStateSaveEmbed(FILE *fp, INT32 nOffset, INT32 bAll);

INT32 BurnStateSave(TCHAR *szName, INT32 bAll);

extern int nSavestateSlot;

int StatedAuto(int bSave);

int StatedLoad(int nSlot);

int StatedSave(int nSlot);

// zipfn.cpp
/*
struct ZipEntry {
    char *szName;
    UINT32 nLen;
    UINT32 nCrc;
};
*/

INT32 ZipOpen(char *szZip);

INT32 ZipClose();

INT32 ZipGetList(struct ZipEntry **pList, INT32 *pnListCount);

INT32 ZipLoadFile(UINT8 *Dest, INT32 nLen, INT32 *pnWrote, INT32 nEntry);

INT32 __cdecl ZipLoadOneFile(char *arcName, const char *fileName, void **Dest, INT32 *pnWrote);

// bzip.cpp
#define BZIP_STATUS_OK        (0)
#define BZIP_STATUS_BADDATA    (1)
#define BZIP_STATUS_ERROR    (2)

INT32 BzipOpen(bool);

INT32 BzipClose();

INT32 BzipInit();

INT32 BzipExit();

INT32 BzipStatus();

// paths.cpp
extern char szAppHomePath[MAX_PATH];
extern char szAppRomPath[MAX_PATH];
extern char szAppSavePath[MAX_PATH];
extern char szAppConfigPath[MAX_PATH];
extern char szAppSamplesPath[MAX_PATH]; // for burn/snd/samples.cpp
extern char szAppPreviewPath[MAX_PATH];
extern char szAppTitlePath[MAX_PATH];
extern char szAppBlendPath[MAX_PATH];
extern char szAppNvPath[MAX_PATH];
extern char szAppSkinPath[MAX_PATH];

void BurnPathsInit();

// drv.cpp
extern int bRunPause;
extern int bDrvOkay; // 1 if the Driver has been initted okay, and it's okay to use the BurnDrv functions
int DrvInit(int nDrvNum, bool bRestore);
int DrvInitCallback(); // Used when Burn library needs to load a game. DrvInit(nBurnSelect, false)
int DrvExit();
int ProgressUpdateBurner(double dProgress, const TCHAR* pszText, bool bAbs);
int AppError(TCHAR* szText, int bWarning);

// main.cpp
extern char szAppBurnVer[16];

// run.cpp
extern bool bPauseOn;

int RunReset();

int RunOneFrame(bool bDraw, int bDrawFps, int fps);

// input.cpp
extern int nAnalogSpeed;

int InpInit();

int InpExit();

void InpDIP();

// stringset.cpp
class StringSet {
public:
    TCHAR *szText;
    int nLen;

    // printf function to add text to the Bzip string
    int __cdecl Add(TCHAR *szFormat, ...);

    int Reset();

    StringSet();

    ~StringSet();
};

#endif // _BURNER_H_

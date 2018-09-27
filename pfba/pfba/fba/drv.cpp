// Driver Init module

#include "c2dui.h"
#include "burner.h"

using namespace c2d;
using namespace c2dui;

extern C2DUIGuiMain *ui;
extern UINT8 NeoSystem;
int bDrvOkay = 0;                        // 1 if the Driver has been initted okay, and it's okay to use the BurnDrv functions
int kNetGame = 0;

static int ProgressCreate();

static UINT8 NeoSystemList[] = {0x0f, 0x0c, 0x0b, 0x0d, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                                0x0a, 0x0e};

static int DoLibInit()                    // Do Init of Burn library driver
{
    int nRet;

    ProgressCreate();

    nRet = BzipOpen(false);
    printf("DoLibInit: BzipOpen = %i\n", nRet);
    if (nRet) {
        BzipClose();
        return 1;
    }

    NeoSystem &= ~(UINT8) 0x1f;
    NeoSystem |= NeoSystemList[ui->getConfig()->getValue(C2DUIOption::Index::ROM_NEOBIOS, true)];

    nRet = BurnDrvInit();
    printf("DoLibInit: BurnDrvInit = %i\n", nRet);

    BzipClose();

    if (nRet) {
        BurnDrvExit();
        return 1;
    } else {
        return 0;
    }
}

// Catch calls to BurnLoadRom() once the emulation has started;
// Intialise the zip module before forwarding the call, and exit cleanly.
static int DrvLoadRom(unsigned char *Dest, int *pnWrote, int i) {

    int nRet;

    BzipOpen(false);

    if ((nRet = BurnExtLoadRom(Dest, pnWrote, i)) != 0) {
        char *pszFilename;

        BurnDrvGetRomName(&pszFilename, i, 0);
        char szText[256] = "";
        sprintf(szText,
                "Error loading %s for %s.\nEmulation will likely have problems.",
                pszFilename, BurnDrvGetTextA(DRV_NAME));
        printf("DrvLoadRom: %s\n", szText);
        ui->getUiMessageBox()->show("ERROR", szText, "OK");
    }

    BzipClose();

    BurnExtLoadRom = DrvLoadRom;

    return nRet;
}

int DrvInit(int nDrvNum, bool bRestore) {

    printf("DrvInit(%i, %i)\n", nDrvNum, bRestore);
    DrvExit();

    nBurnDrvSelect[0] = (UINT32) nDrvNum;

    // Define nMaxPlayers early; GameInpInit() needs it (normally defined in DoLibInit()).
    nMaxPlayers = BurnDrvGetMaxPlayers();

    printf("DrvInit: DoLibInit()\n");
    if (DoLibInit()) {                // Init the Burn library's driver
        //char szTemp[512];
        //_stprintf(szTemp, _T("Error starting '%s'.\n"), BurnDrvGetText(DRV_FULLNAME));
        //AppError(szTemp, 1);
        return 1;
    }

    printf("DrvInit: BurnExtLoadRom = DrvLoadRom\n");
    BurnExtLoadRom = DrvLoadRom;

    char path[1024];
    snprintf(path, 1023, "%s%s.fs", szAppEEPROMPath, BurnDrvGetTextA(DRV_NAME));
    BurnStateLoad(path, 0, NULL);

    bDrvOkay = 1;                    // Okay to use all BurnDrv functions

    nBurnLayer = 0xFF;                // show all layers

    return 0;
}

int DrvInitCallback() {
    return DrvInit(nBurnDrvSelect[0], false);
}

int DrvExit() {
    if (bDrvOkay) {
        if (nBurnDrvSelect[0] < nBurnDrvCount) {
            char path[1024];
            snprintf(path, 1023, "%s%s.fs", szAppEEPROMPath, BurnDrvGetTextA(DRV_NAME));
            BurnStateSave(path, 0);
            BurnDrvExit();                // Exit the driver
        }
    }

    BurnExtLoadRom = NULL;
    bDrvOkay = 0;                    // Stop using the BurnDrv functions
    nBurnDrvSelect[0] = ~0U;            // no driver selected

    return 0;
}

static double nProgressPosBurn = 0;

static int ProgressCreate() {
    nProgressPosBurn = 0;
    ui->getUiProgressBox()->setVisibility(c2d::C2DObject::Visible);
    ui->getUiProgressBox()->setLayer(1000);
    return 0;
}

int ProgressUpdateBurner(double dProgress, const TCHAR *pszText, bool bAbs) {

    ui->getUiProgressBox()->setTitle(BurnDrvGetTextA(DRV_FULLNAME));

    if (pszText) {
        nProgressPosBurn += dProgress;
        ui->getUiProgressBox()->setMessage(pszText);
        ui->getUiProgressBox()->setProgress((float) nProgressPosBurn);
    } else {
        ui->getUiProgressBox()->setMessage("Please wait...");
    }

    ui->getRenderer()->flip();

    return 0;
}

int AppError(TCHAR *szText, int bWarning) {

    //ui->getUiMessageBox()->show("ERROR", szText ? szText : "UNKNOW ERROR", "OK");
    return 1;
}

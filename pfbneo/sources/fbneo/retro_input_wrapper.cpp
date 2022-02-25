//
// Created by cpasjuste on 24/02/2022.
//

#include "cross2d/c2d.h"
#include "retro_input.h"

unsigned nGameType = 0;
bool bIsNeogeoCartGame = false;
bool bLightgunHideCrosshairEnabled = true;
struct GameInp *pgi_reset = nullptr;
struct GameInp *pgi_diag = nullptr;
UINT8 *diag_input = nullptr;
struct GameInp *GameInp = nullptr;
UINT32 nGameInpCount = 0;
INT32 nAnalogSpeed = 0x0100;
INT32 nFireButtons = 0;
static int nDIPOffset;

int16_t input_cb(unsigned port, unsigned device, unsigned index, unsigned id) {

    unsigned int keys = c2d_renderer->getInput()->getPlayer((int) port)->keys;
    //printf("input_cb(joy): port: %i, device: %i, index: %i, id: %x (keys: %x)\n",
    //     port, device, index, id, keys);

    if (device == RETRO_DEVICE_JOYPAD) {
        // diag_input
        if (id == 254 && (keys & c2d::Input::Key::Start)) {
            return 1;
        }
        return keys & id ? 1 : 0;
    }

    return 0;
}

void poll_cb() {}

static void InpDIPSWGetOffset(void) {
    BurnDIPInfo bdi;
    nDIPOffset = 0;

    for (int i = 0; BurnDrvGetDIPInfo(&bdi, i) == 0; i++) {
        if (bdi.nFlags == 0xF0) {
            nDIPOffset = bdi.nInput;
            HandleMessage(RETRO_LOG_INFO, "DIP switches offset: %d.\n", bdi.nInput);
            break;
        }
    }
}

void InpDIPSWResetDIPs(void) {
    int i = 0;
    BurnDIPInfo bdi;
    struct GameInp *pgi = NULL;

    InpDIPSWGetOffset();

    while (BurnDrvGetDIPInfo(&bdi, i) == 0) {
        if (bdi.nFlags == 0xFF) {
            pgi = GameInp + bdi.nInput + nDIPOffset;
            if (pgi)
                pgi->Input.Constant.nConst = (pgi->Input.Constant.nConst & ~bdi.nMask) | (bdi.nSetting & bdi.nMask);
        }
        i++;
    }
}

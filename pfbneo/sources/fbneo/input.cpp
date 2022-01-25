// Module for input using libcross2d

#include "cross2d/c2d.h"
#include "burner.h"
#include "input.h"

using namespace c2d;

bool bAlwaysProcessKeyboardInput = false;

const INT32 nConfigMinVersion = 0x020921;

bool AppProcessKeyboardInput() { return true; }

// input_sdl2.cpp
extern INT32 MapJoystick(struct GameInp *pgi, char *szi, INT32 nPlayer, INT32 nDevice);

extern INT32 display_set_controls();

int buttons[4][8] = {{-1, -1, -1, -1, -1, -1, -1, -1},
                     {-1, -1, -1, -1, -1, -1, -1, -1},
                     {-1, -1, -1, -1, -1, -1, -1, -1},
                     {-1, -1, -1, -1, -1, -1, -1, -1}};

int c2d_mapping[KEY_COUNT + 3] = {
        Input::Key::Up, Input::Key::Down, Input::Key::Left, Input::Key::Right,
        Input::Key::Select, Input::Key::Start,
        Input::Key::Fire1, Input::Key::Fire2, Input::Key::Fire3,
        Input::Key::Fire4, Input::Key::Fire5, Input::Key::Fire6,
        Input::Key::Start, Input::Key::Select,
        C2D_KEY_DIAG,       // virtual key for diag
        C2D_KEY_SERVICE,    // virtual key for service
        C2D_KEY_DIP         // virtual key for dip
};

#define KEY(x) { pgi->nInput = GIT_SWITCH; pgi->Input.Switch.nCode = (UINT16)(x); }

static void C2DGamcMisc(struct GameInp *pgi, char *szi) {

    INT32 nJoyBase = 0x4000;

    if (strcmp(szi, "reset") == 0) {
        // use uiEmu reset (to allow rom settings changes)
    } else if (strcmp(szi, "diag") == 0) {
        KEY(nJoyBase + 0x80 + 14);
    } else if (strcmp(szi, "service") == 0) {
        KEY(nJoyBase + 0x80 + 15);
    } else if (strcmp(szi, "dip") == 0) {
        KEY(nJoyBase + 0x80 + 16);
    }
}

static void C2DGameInpConfigOne(int nPlayer, int nPcDev, struct GameInp *pgi, char *szi) {
    //printf("C2DGameInpConfigOne: player: %i, nPcDev: %i, szi: %s\n", nPlayer, nPcDev, szi);
    GamcPlayer(pgi, szi, nPlayer, nPcDev);
    GamcAnalogJoy(pgi, szi, nPlayer, nPcDev, 1);
    MapJoystick(pgi, szi, nPlayer, nPcDev + 1);
    if (nPlayer == 0) {
        C2DGamcMisc(pgi, szi);
    }
}

static int C2DGameInpConfig(int nPlayer, int nPcDev) {
    struct GameInp *pgi;
    unsigned int i;

    for (i = 0, pgi = GameInp; i < nGameInpCount; i++, pgi++) {
        struct BurnInputInfo bii{};
        // Get the extra info about the input
        bii.szInfo = nullptr;
        BurnDrvGetInputInfo(&bii, i);
        if (bii.pVal == nullptr) {
            continue;
        }
        if (bii.szInfo == nullptr) {
            bii.szInfo = "";
        }
        C2DGameInpConfigOne(nPlayer, nPcDev, pgi, bii.szInfo);
    }

    for (i = 0; i < nMacroCount; i++, pgi++) {
        C2DGameInpConfigOne(nPlayer, nPcDev, pgi, pgi->Macro.szName);
    }

    GameInpCheckLeftAlt();

    return 0;
}

int C2DInpInit() {
    for (auto &button: buttons) {
        // see C2D_DEFAULT_JOY_KEYS in c2d.h
        button[0] = 6;  // KEY_JOY_FIRE1_DEFAULT
        button[1] = 7;  // KEY_JOY_FIRE2_DEFAULT
        button[2] = 8;  // KEY_JOY_FIRE3_DEFAULT
        button[3] = 9;  // KEY_JOY_FIRE4_DEFAULT
        button[4] = 10; // KEY_JOY_FIRE5_DEFAULT
        button[5] = 11; // KEY_JOY_FIRE6_DEFAULT
        button[6] = 4;  // KEY_JOY_COIN1_DEFAULT
        button[7] = 5;  // KEY_JOY_START1_DEFAULT
    }

    for (int i = 0; i < PLAYER_MAX; ++i) {
        C2DGameInpConfig(i, i);
    }

    display_set_controls();

    return 0;
}

// Read one joystick axis
int C2DInpJoyAxis(int i, int nAxis) {

    if (i < 0 || i >= PLAYER_MAX) {
        //printf("C2DInpJoyAxis: player not found (%i)\n", i);
        return 0;
    }

    if (nAxis > 3) {
        //printf("C2DInpJoyAxis: axis not found (%i)\n", nAxis);
        return 0;
    }

    auto player = c2d_renderer->getInput()->getPlayer(i);
    switch (nAxis) {
        default:
            return player->lx.value;
        case 1:
            return player->ly.value;
        case 2:
            return player->rx.value;
        case 3:
            return player->ry.value;
    }
}

// Check a subcode (the 40xx bit in 4001, 4102 etc) for a joystick input code
static int C2DJoystickState(int i, int nSubCode) {

    if (i < 0 || i >= PLAYER_MAX) {
        return 0;
    }

    auto player = c2d_renderer->getInput()->getPlayer(i);

    // Joystick directions
    if (nSubCode < 0x10) {
        // ?? (from inp_sdl2.cpp)
        if (8 <= nSubCode) {
            return 0;
        }

        switch (nSubCode) {
            case 0x00:
                return ((player->keys & Input::Key::Left) ? 1 : 0);
            case 0x01:
                return ((player->keys & Input::Key::Right) ? 1 : 0);
            case 0x02:
                return ((player->keys & Input::Key::Up) ? 1 : 0);
            case 0x03:
                return ((player->keys & Input::Key::Down) ? 1 : 0);
            default:
                printf("C2DJoystickState: nSubCode not handled (0x%08x)\n", nSubCode);
                return 0;
        }
    }

    // POV hat controls
    if (nSubCode < 0x20) {
        //printf("POV: nSubCode = 0x%08x\n", nSubCode & 3);
        switch (nSubCode & 3) {
            case 0:
                return ((player->keys & Input::Key::Left) ? 1 : 0);
            case 1:
                return ((player->keys & Input::Key::Right) ? 1 : 0);
            case 2:
                return ((player->keys & Input::Key::Up) ? 1 : 0);
            case 3:
                return ((player->keys & Input::Key::Down) ? 1 : 0);
        }

        return 0;
    }

    // Undefined
    if (nSubCode < 0x80) {
        return 0;
    }

    // Joystick buttons
    if (nSubCode < 0x80 + (KEY_COUNT + 3)) {
        return ((player->keys & c2d_mapping[nSubCode & 0x7F]) ? 1 : 0);
    }

    return 0;
}

// Get the state (pressed = 1, not pressed = 0) of a particular input code
int C2DInpState(int nCode) {

    if (nCode < 0) {
        //printf("C2DInpState: nCode < 0\n");
        return 0;
    }

    if (nCode < 0x100) {
        //printf("C2DInpState: keyboard\n");
        return 0;
    }

    if (nCode < 0x4000) {
        //printf("C2DInpState: nCode < 0x4000\n");
        return 0;
    }

    if (nCode < 0x8000) {
        // Codes 4000-8000 = Joysticks
        int nJoyNumber = (nCode - 0x4000) >> 8;
        // Find the joystick state in our array
        return C2DJoystickState(nJoyNumber, nCode & 0xFF);
    }

    if (nCode < 0xC000) {
        // Codes 8000-C000 = Mouse
        //printf("C2DInpState: mouse\n");
        return 0;
    }

    return 0;
}

int C2DInpStart() {
    return 0;
}

int C2DInpMouseAxis(int, int) {

    return 0;
}

int C2DInpFind(bool) {
    return -1;
}

int C2DInpGetControlName(int, TCHAR *, TCHAR *) {
    return 0;
}

int C2DInpSetCooperativeLevel(bool, bool) {
    return 0;
}

int C2DInpExit() {
    return 0;
}

struct InputInOut InputInOutSDL2 = {C2DInpInit, C2DInpExit, C2DInpSetCooperativeLevel,
                                    C2DInpStart, C2DInpState, C2DInpJoyAxis, C2DInpMouseAxis,
                                    C2DInpFind, C2DInpGetControlName, nullptr,
                                    _T("c2d input")};

// Module for input using libcross2d

#include "cross2d/c2d.h"
#include "burner.h"

using namespace c2d;

int nVidFullscreen = 0;
bool bAlwaysProcessKeyboardInput = false;
INT32 bVidUseHardwareGamma = 1;
const INT32 nConfigMinVersion = 0x020921;

// TODO: this does not belong here...
UINT32 (__cdecl *VidHighCol)(INT32 r, INT32 g, INT32 b, INT32 i);

// TODO: this does not belong here...
INT32 VidRecalcPal() { return BurnRecalcPal(); }

bool AppProcessKeyboardInput() { return true; }

int buttons[4][8] = {{-1, -1, -1, -1, -1, -1, -1, -1},
                     {-1, -1, -1, -1, -1, -1, -1, -1},
                     {-1, -1, -1, -1, -1, -1, -1, -1},
                     {-1, -1, -1, -1, -1, -1, -1, -1}};

int c2d_mapping[KEY_COUNT] = {
        Input::Key::Up, Input::Key::Down, Input::Key::Left, Input::Key::Right,
        Input::Key::Select, Input::Key::Start,
        Input::Key::Fire1, Input::Key::Fire2, Input::Key::Fire3,
        Input::Key::Fire4, Input::Key::Fire5, Input::Key::Fire6,
        Input::Key::Start, Input::Key::Select
};

int C2DInpInit() {
    for (int i = 0; i < PLAYER_MAX; i++) {
        // see C2D_DEFAULT_JOY_KEYS in c2d.h
        buttons[i][0] = 6;  // KEY_JOY_FIRE1_DEFAULT
        buttons[i][1] = 7;  // KEY_JOY_FIRE2_DEFAULT
        buttons[i][2] = 8;  // KEY_JOY_FIRE3_DEFAULT
        buttons[i][3] = 9;  // KEY_JOY_FIRE4_DEFAULT
        buttons[i][4] = 10; // KEY_JOY_FIRE5_DEFAULT
        buttons[i][5] = 11; // KEY_JOY_FIRE6_DEFAULT
        buttons[i][6] = 4;  // KEY_JOY_COIN1_DEFAULT
        buttons[i][7] = 5;  // KEY_JOY_START1_DEFAULT
    }
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
    if (nSubCode < 0x80 + KEY_COUNT) {
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

int C2DInpMouseAxis(int i, int nAxis) {
    return 0;
}

int C2DInpFind(bool CreateBaseline) {
    return -1;
}

int C2DInpGetControlName(int nCode, TCHAR *pszDeviceName, TCHAR *pszControlName) {
    return 0;
}

int C2DInpSetCooperativeLevel(bool bExclusive, bool /*bForeGround*/) {
    return 0;
}

int C2DInpExit() {
    return 0;
}

struct InputInOut InputInOutSDL2 = {C2DInpInit, C2DInpExit, C2DInpSetCooperativeLevel,
                                    C2DInpStart, C2DInpState, C2DInpJoyAxis, C2DInpMouseAxis,
                                    C2DInpFind, C2DInpGetControlName, nullptr,
                                    _T("c2d input")};

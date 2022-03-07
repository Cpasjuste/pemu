//
// Created by cpasjuste on 24/02/2022.
//

#ifndef PEMU_RETRO_INPUT_WRAPPER_H
#define PEMU_RETRO_INPUT_WRAPPER_H

#include <cstdio>
#include <string>
#include <vector>
#include "burner.h"
#include "cross2d/c2d.h"

extern bool bIsNeogeoCartGame;
extern unsigned nGameType;
extern bool bLightgunHideCrosshairEnabled;

extern struct GameInp *pgi_reset;
extern struct GameInp *pgi_diag;
extern UINT8 *diag_input;

int16_t input_cb(unsigned port, unsigned device, unsigned index, unsigned id);

void poll_cb();

#define HandleMessage(x, ...) printf(__VA_ARGS__)
#define log_cb HandleMessage
#define environ_cb(x, y) (void*)NULL
#define SSTR(x) "..."

#define RETRO_GAME_TYPE_NEOCD    13
#define HARDWARE_SEGA_SYSTEM32 (HARDWARE_PREFIX_SEGA | 0x000c0000)
#define HARDWARE_NVS (HARDWARE_PREFIX_NES | 0x00010000)

#define RETRO_DEVICE_TYPE_SHIFT         8
#define RETRO_DEVICE_MASK               ((1 << RETRO_DEVICE_TYPE_SHIFT) - 1)
#define RETRO_DEVICE_SUBCLASS(base, id) (((id + 1) << RETRO_DEVICE_TYPE_SHIFT) | base)

#define RETRO_DEVICE_NONE         0
#define RETRO_DEVICE_JOYPAD       1
#define RETRO_DEVICE_MOUSE        2
#define RETRO_DEVICE_KEYBOARD     3
#define RETRO_DEVICE_LIGHTGUN     4
#define RETRO_DEVICE_ANALOG       5
#define RETRO_DEVICE_POINTER      6

#ifdef __SWITCH__
#define RETRO_DEVICE_ID_JOYPAD_B        c2d::Input::Key::Fire2
#define RETRO_DEVICE_ID_JOYPAD_A        c2d::Input::Key::Fire1
#define RETRO_DEVICE_ID_JOYPAD_Y        c2d::Input::Key::Fire4
#define RETRO_DEVICE_ID_JOYPAD_X        c2d::Input::Key::Fire3
#else
#define RETRO_DEVICE_ID_JOYPAD_B        c2d::Input::Key::Fire1
#define RETRO_DEVICE_ID_JOYPAD_A        c2d::Input::Key::Fire2
#define RETRO_DEVICE_ID_JOYPAD_Y        c2d::Input::Key::Fire3
#define RETRO_DEVICE_ID_JOYPAD_X        c2d::Input::Key::Fire4
#endif
#define RETRO_DEVICE_ID_JOYPAD_SELECT   c2d::Input::Key::Select
#define RETRO_DEVICE_ID_JOYPAD_START    c2d::Input::Key::Start
#define RETRO_DEVICE_ID_JOYPAD_UP       c2d::Input::Key::Up
#define RETRO_DEVICE_ID_JOYPAD_DOWN     c2d::Input::Key::Down
#define RETRO_DEVICE_ID_JOYPAD_LEFT     c2d::Input::Key::Left
#define RETRO_DEVICE_ID_JOYPAD_RIGHT    c2d::Input::Key::Right
#define RETRO_DEVICE_ID_JOYPAD_L        c2d::Input::Key::Fire5
#define RETRO_DEVICE_ID_JOYPAD_R        c2d::Input::Key::Fire6
#define RETRO_DEVICE_ID_JOYPAD_L2       c2d::Input::Key::Fire7
#define RETRO_DEVICE_ID_JOYPAD_R2       c2d::Input::Key::Fire8
#define RETRO_DEVICE_ID_JOYPAD_L3       BIT(21)
#define RETRO_DEVICE_ID_JOYPAD_R3       BIT(22)

#define RETRO_DEVICE_ID_JOYPAD_MASK    256

/* Index / Id values for ANALOG device. */
#define RETRO_DEVICE_INDEX_ANALOG_LEFT       0
#define RETRO_DEVICE_INDEX_ANALOG_RIGHT      1
#define RETRO_DEVICE_INDEX_ANALOG_BUTTON     2
#define RETRO_DEVICE_ID_ANALOG_X             0
#define RETRO_DEVICE_ID_ANALOG_Y             1

/* Id values for MOUSE. */
#define RETRO_DEVICE_ID_MOUSE_X                0
#define RETRO_DEVICE_ID_MOUSE_Y                1
#define RETRO_DEVICE_ID_MOUSE_LEFT             2
#define RETRO_DEVICE_ID_MOUSE_RIGHT            3
#define RETRO_DEVICE_ID_MOUSE_WHEELUP          4
#define RETRO_DEVICE_ID_MOUSE_WHEELDOWN        5
#define RETRO_DEVICE_ID_MOUSE_MIDDLE           6
#define RETRO_DEVICE_ID_MOUSE_HORIZ_WHEELUP    7
#define RETRO_DEVICE_ID_MOUSE_HORIZ_WHEELDOWN  8
#define RETRO_DEVICE_ID_MOUSE_BUTTON_4         9
#define RETRO_DEVICE_ID_MOUSE_BUTTON_5         10

/* Id values for LIGHTGUN. */
#define RETRO_DEVICE_ID_LIGHTGUN_SCREEN_X        13 /*Absolute Position*/
#define RETRO_DEVICE_ID_LIGHTGUN_SCREEN_Y        14 /*Absolute*/
#define RETRO_DEVICE_ID_LIGHTGUN_IS_OFFSCREEN    15 /*Status Check*/
#define RETRO_DEVICE_ID_LIGHTGUN_TRIGGER          2
#define RETRO_DEVICE_ID_LIGHTGUN_RELOAD          16 /*Forced off-screen shot*/
#define RETRO_DEVICE_ID_LIGHTGUN_AUX_A            3
#define RETRO_DEVICE_ID_LIGHTGUN_AUX_B            4
#define RETRO_DEVICE_ID_LIGHTGUN_START            6
#define RETRO_DEVICE_ID_LIGHTGUN_SELECT           7
#define RETRO_DEVICE_ID_LIGHTGUN_AUX_C            8
#define RETRO_DEVICE_ID_LIGHTGUN_DPAD_UP          9
#define RETRO_DEVICE_ID_LIGHTGUN_DPAD_DOWN       10
#define RETRO_DEVICE_ID_LIGHTGUN_DPAD_LEFT       11
#define RETRO_DEVICE_ID_LIGHTGUN_DPAD_RIGHT      12
/* deprecated */
#define RETRO_DEVICE_ID_LIGHTGUN_X                0 /*Relative Position*/
#define RETRO_DEVICE_ID_LIGHTGUN_Y                1 /*Relative*/
#define RETRO_DEVICE_ID_LIGHTGUN_CURSOR           3 /*Use Aux:A*/
#define RETRO_DEVICE_ID_LIGHTGUN_TURBO            4 /*Use Aux:B*/
#define RETRO_DEVICE_ID_LIGHTGUN_PAUSE            5 /*Use Start*/

/* Id values for POINTER. */
#define RETRO_DEVICE_ID_POINTER_X         0
#define RETRO_DEVICE_ID_POINTER_Y         1
#define RETRO_DEVICE_ID_POINTER_PRESSED   2
#define RETRO_DEVICE_ID_POINTER_COUNT     3

struct retro_input_descriptor {
    unsigned port;
    unsigned device;
    unsigned index;
    unsigned id;
    const char *description;
};

struct retro_controller_description {
    const char *desc;
    unsigned id;
};

struct retro_controller_info {
    const struct retro_controller_description *types;
    unsigned num_types;
};

enum retro_key {
    RETROK_UNKNOWN = 0,
    RETROK_FIRST = 0,
    RETROK_BACKSPACE = 8,
    RETROK_TAB = 9,
    RETROK_CLEAR = 12,
    RETROK_RETURN = 13,
    RETROK_PAUSE = 19,
    RETROK_ESCAPE = 27,
    RETROK_SPACE = 32,
    RETROK_EXCLAIM = 33,
    RETROK_QUOTEDBL = 34,
    RETROK_HASH = 35,
    RETROK_DOLLAR = 36,
    RETROK_AMPERSAND = 38,
    RETROK_QUOTE = 39,
    RETROK_LEFTPAREN = 40,
    RETROK_RIGHTPAREN = 41,
    RETROK_ASTERISK = 42,
    RETROK_PLUS = 43,
    RETROK_COMMA = 44,
    RETROK_MINUS = 45,
    RETROK_PERIOD = 46,
    RETROK_SLASH = 47,
    RETROK_0 = 48,
    RETROK_1 = 49,
    RETROK_2 = 50,
    RETROK_3 = 51,
    RETROK_4 = 52,
    RETROK_5 = 53,
    RETROK_6 = 54,
    RETROK_7 = 55,
    RETROK_8 = 56,
    RETROK_9 = 57,
    RETROK_COLON = 58,
    RETROK_SEMICOLON = 59,
    RETROK_LESS = 60,
    RETROK_EQUALS = 61,
    RETROK_GREATER = 62,
    RETROK_QUESTION = 63,
    RETROK_AT = 64,
    RETROK_LEFTBRACKET = 91,
    RETROK_BACKSLASH = 92,
    RETROK_RIGHTBRACKET = 93,
    RETROK_CARET = 94,
    RETROK_UNDERSCORE = 95,
    RETROK_BACKQUOTE = 96,
    RETROK_a = 97,
    RETROK_b = 98,
    RETROK_c = 99,
    RETROK_d = 100,
    RETROK_e = 101,
    RETROK_f = 102,
    RETROK_g = 103,
    RETROK_h = 104,
    RETROK_i = 105,
    RETROK_j = 106,
    RETROK_k = 107,
    RETROK_l = 108,
    RETROK_m = 109,
    RETROK_n = 110,
    RETROK_o = 111,
    RETROK_p = 112,
    RETROK_q = 113,
    RETROK_r = 114,
    RETROK_s = 115,
    RETROK_t = 116,
    RETROK_u = 117,
    RETROK_v = 118,
    RETROK_w = 119,
    RETROK_x = 120,
    RETROK_y = 121,
    RETROK_z = 122,
    RETROK_LEFTBRACE = 123,
    RETROK_BAR = 124,
    RETROK_RIGHTBRACE = 125,
    RETROK_TILDE = 126,
    RETROK_DELETE = 127,

    RETROK_KP0 = 256,
    RETROK_KP1 = 257,
    RETROK_KP2 = 258,
    RETROK_KP3 = 259,
    RETROK_KP4 = 260,
    RETROK_KP5 = 261,
    RETROK_KP6 = 262,
    RETROK_KP7 = 263,
    RETROK_KP8 = 264,
    RETROK_KP9 = 265,
    RETROK_KP_PERIOD = 266,
    RETROK_KP_DIVIDE = 267,
    RETROK_KP_MULTIPLY = 268,
    RETROK_KP_MINUS = 269,
    RETROK_KP_PLUS = 270,
    RETROK_KP_ENTER = 271,
    RETROK_KP_EQUALS = 272,

    RETROK_UP = 273,
    RETROK_DOWN = 274,
    RETROK_RIGHT = 275,
    RETROK_LEFT = 276,
    RETROK_INSERT = 277,
    RETROK_HOME = 278,
    RETROK_END = 279,
    RETROK_PAGEUP = 280,
    RETROK_PAGEDOWN = 281,

    RETROK_F1 = 282,
    RETROK_F2 = 283,
    RETROK_F3 = 284,
    RETROK_F4 = 285,
    RETROK_F5 = 286,
    RETROK_F6 = 287,
    RETROK_F7 = 288,
    RETROK_F8 = 289,
    RETROK_F9 = 290,
    RETROK_F10 = 291,
    RETROK_F11 = 292,
    RETROK_F12 = 293,
    RETROK_F13 = 294,
    RETROK_F14 = 295,
    RETROK_F15 = 296,

    RETROK_NUMLOCK = 300,
    RETROK_CAPSLOCK = 301,
    RETROK_SCROLLOCK = 302,
    RETROK_RSHIFT = 303,
    RETROK_LSHIFT = 304,
    RETROK_RCTRL = 305,
    RETROK_LCTRL = 306,
    RETROK_RALT = 307,
    RETROK_LALT = 308,
    RETROK_RMETA = 309,
    RETROK_LMETA = 310,
    RETROK_LSUPER = 311,
    RETROK_RSUPER = 312,
    RETROK_MODE = 313,
    RETROK_COMPOSE = 314,

    RETROK_HELP = 315,
    RETROK_PRINT = 316,
    RETROK_SYSREQ = 317,
    RETROK_BREAK = 318,
    RETROK_MENU = 319,
    RETROK_POWER = 320,
    RETROK_EURO = 321,
    RETROK_UNDO = 322,
    RETROK_OEM_102 = 323,

    RETROK_LAST,

    RETROK_DUMMY = INT_MAX /* Ensure sizeof(enum) == sizeof(int) */
};

#endif //PEMU_RETRO_INPUT_WRAPPER_H

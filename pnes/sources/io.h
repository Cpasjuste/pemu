//
// Created by cpasjuste on 17/05/19.
//

#ifndef PEMU_IO_H
#define PEMU_IO_H

// TODO: fix this on switch (nstcommon.cpp => ifdef _MINGW)
#ifdef __SWITCH__
#include <strings.h>
#endif

// TODO: fix this on windows (nstcommon.cpp => ifdef _MINGW)
#ifdef __WINDOWS__
extern "C" {
int __cdecl mkdir(const char *) __MINGW_ATTRIB_DEPRECATED_MSVC2005;
}
#else
#define mkdir(x) mkdir(x, 0777)
#endif

#endif //PEMU_IO_H

//
// Created by cpasjuste on 29/09/2023.
//

#ifndef PEMU_VFS_H
#define PEMU_VFS_H

#include <io.h>
#undef mkdir
#define mkdir(x, y) mkdir(x)
#include "../../../../../../external/cores/mgba/include/mgba-util/vfs.h"

#endif //PEMU_VFS_H

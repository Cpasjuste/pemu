//
// Created by cpasjuste on 28/05/18.
//

#ifndef PEMU_H
#define PEMU_H

#include "ss_api.h"
#include "cross2d/c2d.h"

#ifdef __MPV__

#include "mpv.h"
#include "mpv_texture.h"

#endif

#include "romlist.h"
#include "skin.h"
#include "ui_video.h"
#include "pemu_config.h"

#include "skin/SkinnedRectangle.h"
#include "skin/SkinnedText.h"

#include "ui_listbox.h"
#include "ui_highlight.h"
#include "ui_menu.h"
#include "ui_emu.h"
#include "ui_romlist.h"
#include "ui_romlist_rominfo.h"
#include "ui_progressbox.h"
#include "ui_menu_state.h"
#include "ui_status_box.h"
#include "ui_main.h"
#include "ui_help.h"

using namespace c2d;
using namespace pemu;
using namespace ss_api;

#endif //PEMU_H

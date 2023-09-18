//
// Created by cpasjuste on 28/05/18.
//

#ifndef C2DUI_H
#define C2DUI_H

#include "ss_api.h"
#include "cross2d/c2d.h"

#ifdef __MPV__

#include "mpv.h"
#include "mpv_texture.h"

#endif

#include "c2dui_romlist.h"
#include "c2dui_skin.h"
#include "c2dui_video.h"
#include "c2dui_config.h"

#include "skin/SkinnedRectangle.h"
#include "skin/SkinnedText.h"

#include "c2dui_ui_listbox.h"
#include "c2dui_ui_highlight.h"
#include "c2dui_ui_menu.h"
#include "c2dui_ui_emu.h"
#include "c2dui_ui_romlist.h"
#include "c2dui_ui_romlist_rominfo.h"
#include "c2dui_ui_progressbox.h"
#include "c2dui_ui_menu_state.h"
#include "c2dui_status_box.h"
#include "c2dui_ui_main.h"
#include "c2dui_ui_help.h"

using namespace c2d;
using namespace c2dui;
using namespace ss_api;

#endif //C2DUI_H

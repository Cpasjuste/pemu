//
// Created by cpasjuste on 01/12/16.
//

#include "c2dui.h"
#include "video.h"

using namespace c2d;
using namespace c2dui;

#define BDF_ORIENTATION_FLIPPED     (1 << 1)
#define BDF_ORIENTATION_VERTICAL    (1 << 2)

extern "C" int BurnDrvGetFlags();

PFBAVideo::PFBAVideo(UiMain *ui, void **_pixels, int *_pitch, const c2d::Vector2f &size, const c2d::Vector2i &aspect)
        : C2DUIVideo(ui, _pixels, _pitch, size, aspect) {
}

void PFBAVideo::updateScaling(bool vertical, bool flip) {

    bool v = (bool) (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL);
    bool f = (bool) (BurnDrvGetFlags() & BDF_ORIENTATION_FLIPPED);

    C2DUIVideo::updateScaling(v, f);
}

//
// Created by cpasjuste on 01/12/16.
//

#include "c2dui.h"
#include "video.h"
#include "burner.h"

using namespace c2d;
using namespace c2dui;

PFBAVideo::PFBAVideo(UiMain *ui, uint8_t **_pixels, int *_pitch, const c2d::Vector2i &size, const c2d::Vector2i &aspect)
        : C2DUIVideo(ui, _pixels, _pitch, size, aspect) {
}

void PFBAVideo::updateScaling(bool vertical, bool flip) {
    bool v = (bool) (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL);
    bool f = (bool) (BurnDrvGetFlags() & BDF_ORIENTATION_FLIPPED);
    C2DUIVideo::updateScaling(v, f);
}

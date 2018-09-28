//
// Created by cpasjuste on 01/12/16.
//

#include "c2dui.h"
#include "video.h"

using namespace c2d;
using namespace c2dui;

extern bool snes9x_height_extended;

PSNESVideo::PSNESVideo(UIMain *gui, void **_pixels, int *_pitch, const c2d::Vector2f &size)
        : C2DUIVideo(gui, _pixels, _pitch, size) {
    ui = gui;
}

void PSNESVideo::updateScaling(bool vertical, bool flip) {

    int high_res = ui->getConfig()->getValue(Option::ROM_HIGH_RES, true);
    int scale_mode = ui->getConfig()->getValue(Option::ROM_SCALING, true);
    Vector2f screen = ui->getRenderer()->getSize();
    Vector2f scale_max;
    float sx = 1, sy = 1, factor;
    bool integer_scaling = false;

    scale_max.x = screen.x / getTextureRect().width;
    scale_max.y = screen.y / getTextureRect().height;

    switch (scale_mode) {

        default:
            // 2x in non "high res" mode (2x software scaling already applied in high res mode)
            if (!high_res) {
                sx = sy = std::min(scale_max.x, 2.0f);
                if (sy > scale_max.y) {
                    sx = sy = std::min(scale_max.y, 2.0f);
                }
            }
            integer_scaling = true;
            break;

        case 1:
            // 3x (2x software scaling already applied in high res mode)
            factor = high_res ? 1.5f : 3.0f;
            sx = sy = std::min(scale_max.x, factor);
            if (sy > scale_max.y) {
                sx = sy = std::min(scale_max.y, factor);
            }
            integer_scaling = true;
            break;

        case 2: // fit
            sx = sy = scale_max.y;
            if (sx > scale_max.x) {
                sx = sy = scale_max.x;
            }
            break;

        case 3: // fit 4:3
            sy = scale_max.y;
            sx = std::min(scale_max.x, (sy * getTextureRect().height * 1.33f) / getTextureRect().width);
            break;

        case 4: // fullscreen
            sx = scale_max.x;
            sy = scale_max.y;
            break;
    }

    setOriginCenter();
    // remove snes9x border if needed
    float posY = snes9x_height_extended ? screen.y / 2 : (screen.y / 2) * 1.065f;
    float scaleY = snes9x_height_extended || integer_scaling ? sy : sy * 1.065f;
    setPosition(screen.x / 2, posY);
    setScale(sx, scaleY);
}

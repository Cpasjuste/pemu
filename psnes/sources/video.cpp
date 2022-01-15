//
// Created by cpasjuste on 01/12/16.
//

#include "c2dui.h"
#include "video.h"

using namespace c2d;
using namespace c2dui;

extern bool snes9x_height_extended;

PSNESVideo::PSNESVideo(UiMain *gui, void **_pixels, int *_pitch, const c2d::Vector2f &size)
        : C2DUIVideo(gui, _pixels, _pitch, size) {
    ui = gui;
}

void PSNESVideo::updateScaling(bool vertical, bool flip) {

    // TODO: verify config
    bool high_res = ui->getConfig()->get(Option::ROM_PSNES_HIGH_RES, true)->getValueBool();
    std::string scale_mode = ui->getConfig()->get(Option::ROM_SCALING, true)->getValueString();
    Vector2f screen = ui->getSize();
    Vector2f scale_max;
    float sx = 1, sy = 1, factor;
    bool integer_scaling = false;

    scale_max.x = screen.x / (float) getTextureRect().width;
    scale_max.y = screen.y / (float) getTextureRect().height;

    if (scale_mode == "NONE") {
        integer_scaling = true;
    } else if (scale_mode == "2X") {
        // 2x in non "high res" mode (2x software scaling already applied in high res mode)
        if (!high_res) {
            sx = sy = std::min(scale_max.x, 2.0f);
            if (sy > scale_max.y) {
                sx = sy = std::min(scale_max.y, 2.0f);
            }
        }
        integer_scaling = true;
    } else if (scale_mode == "3X") {
        // 3x (2x software scaling already applied in high res mode)
        factor = high_res ? 1.5f : 3.0f;
        sx = sy = std::min(scale_max.x, factor);
        if (sy > scale_max.y) {
            sx = sy = std::min(scale_max.y, factor);
        }
        integer_scaling = true;
    } else if (scale_mode == "4X") {
        // 4x (2x software scaling already applied in high res mode)
        factor = high_res ? 2.0f : 4.0f;
        sx = sy = std::min(scale_max.x, factor);
        if (sy > scale_max.y) {
            sx = sy = std::min(scale_max.y, factor);
        }
        integer_scaling = true;
    } else if (scale_mode == "FIT") {
        sx = sy = scale_max.y;
        if (sx > scale_max.x) {
            sx = sy = scale_max.x;
        }
    } else if (scale_mode == "FIT 4:3") {
        sy = scale_max.y;
        sx = std::min(scale_max.x, (sy * (float) getTextureRect().height * 1.33f) / (float) getTextureRect().width);
    } else if (scale_mode == "FULL") {
        sx = scale_max.x;
        sy = scale_max.y;
    }

    setOrigin(Origin::Center);
    // remove snes9x border if needed
    float posY = snes9x_height_extended ? screen.y / 2 : (screen.y / 2) * 1.065f;
    float scaleY = snes9x_height_extended || integer_scaling ? sy : sy * 1.065f;
    setPosition(screen.x / 2, posY);
    setScale(sx, scaleY);
}

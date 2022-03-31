//
// Created by cpasjuste on 01/12/16.
//

#include "c2dui.h"
#include "video.h"
#include "snes9x.h"

using namespace c2d;
using namespace c2dui;

extern Vector2f snes9x_game_size;

PSNESVideo::PSNESVideo(UiMain *gui, uint8_t **_pixels, int *_pitch, const c2d::Vector2i &size)
        : C2DUIVideo(gui, _pixels, _pitch, size) {
    ui = gui;
}

void PSNESVideo::updateScaling(bool vertical, bool flip) {
    std::string scale_value = ui->getConfig()->get(Option::Id::ROM_SCALING, true)->getValueString();
    float scale_value_float = (float) ui->getConfig()->get(Option::Id::ROM_SCALING, true)->getIndex() + 1;
    std::string scaling_mode = ui->getConfig()->get(Option::Id::ROM_SCALING_MODE, true)->getValueString();
    float game_aspect_ratio = (float) aspect.x / (float) aspect.y;

    Vector2f screen = ui->getSize();
    Vector2f scale_max{screen.x / snes9x_game_size.x, screen.y / snes9x_game_size.y};
    Vector2f scale;

    if (scale_value == "FULL") {
        scale = scale_max;
    } else {
        // scale_value_float 4 == FIT
        scale.y = scale_value_float == 4 ? scale_max.y : std::min(scale_max.y, scale_value_float);
        if (scaling_mode == "ASPECT") {
            float size_x = ((float) snes9x_game_size.y * scale.y) * game_aspect_ratio;
            scale.x = size_x / (float) snes9x_game_size.x;
        } else {
            // integer scaling
            scale.x = scale.y;
        }
    }

    printf("C2DUIVideo::updateScaling: mode: %s, scaling: %f x %f, size: %i x %i, game size: %i x %i\n",
           scale_value.c_str(), scale.x, scale.y,
           (int) (snes9x_game_size.x * scale.x), (int) (snes9x_game_size.y * scale.y),
           (int) snes9x_game_size.x, (int) snes9x_game_size.y);

    setPosition(screen.x / 2, (screen.y / 2) + (((getSize().y - snes9x_game_size.y) / 2) * scale.y));
    setOrigin(Origin::Center);
    setScale(scale);
}
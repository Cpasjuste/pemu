//
// Created by cpasjuste on 01/12/16.
//

#include "c2dui.h"

C2DUIVideo::C2DUIVideo(UiMain *u, void **_pixels, int *_pitch,
                       const c2d::Vector2f &size, const c2d::Vector2i &a, Texture::Format format)
        : C2DTexture(size, format) {
    ui = u;
    aspect = a;

    if (_pixels != nullptr) {
        auto rect = (FloatRect) getTextureRect();
        C2DTexture::lock(&rect, _pixels, _pitch);
        C2DTexture::unlock();
    }
}

void C2DUIVideo::updateScaling(bool vertical, bool flip) {

    bool rotated = false;
    float rotation = 0;
    int rotation_cfg = 0;
    std::string scale_value = ui->getConfig()->get(Option::Id::ROM_SCALING, true)->getValueString();
    float scale_value_float = (float) ui->getConfig()->get(Option::Id::ROM_SCALING, true)->getIndex() + 1;
    std::string scaling_mode = ui->getConfig()->get(Option::Id::ROM_SCALING_MODE, true)->getValueString();
    float game_aspect_ratio = vertical ? (float) aspect.y / (float) aspect.x
                                       : (float) aspect.x / (float) aspect.y;
    Option *rotationOpt = ui->getConfig()->get(Option::Id::ROM_ROTATION, true);
    if (rotationOpt) {
        rotation_cfg = rotationOpt->getIndex();
    }

    Vector2f screen = ui->getSize();
    Vector2f scale_max;
    Vector2f scale;

#ifndef __PSP2__
    if (vertical) {
        switch (rotation_cfg) {
            case 1: // ON
                rotation = flip ? 90 : 270;
                rotated = true;
                break;
            case 2: // CAB MODE
                rotation = flip ? 0 : 180;
                break;
            default: // OFF
                rotation = flip ? 180 : 0;
                break;
        }
    }
#else
    // TODO: force right to left orientation on psp2,
    // should add platform specific code
    if ((rotation_cfg == 0 || rotation_cfg == 3) && vertical) {
        if (!flip) {
            rotation = 180;
        }
    } else if (rotation_cfg == 2 && vertical) {
        if (flip) {
            rotation = 180;
        }
    } else {
        if (flip) {
            rotation = 90;
            rotated = 1;
        } else if (vertical) {
            rotation = -90;
            rotated = 1;
        } else {
            rotation = 0;
        }
    }
#endif

    if (rotated) {
        scale_max.x = screen.x / (float) getTextureRect().height;
        scale_max.y = screen.y / (float) getTextureRect().width;
    } else {
        scale_max.x = screen.x / (float) getTextureRect().width;
        scale_max.y = screen.y / (float) getTextureRect().height;
    }

    if (scale_value == "FULL") {
        scale.x = rotated ? scale_max.y : scale_max.x;
        scale.y = rotated ? scale_max.x : scale_max.y;
    } else {
        if (!rotated) {
            // scale_value_float 4 == FIT
            scale.y = scale_value_float == 4 ? scale_max.y : std::min(scale_max.y, scale_value_float);
            float size_x = ((float) getTextureRect().height * scale.y) * game_aspect_ratio;
            if (scaling_mode == "AUTO") {
                scale.x = size_x / (float) getTextureRect().width;
                // use integer scaling if aspect ratio is not too divergent
                if (scale.y / scale.x < 1.2f && scale.y / scale.x > 0.8f) {
                    scale.x = scale.y;
                }
            } else if (scaling_mode == "ASPECT") {
                scale.x = size_x / (float) getTextureRect().width;
            } else {
                // integer scaling
                scale.x = scale.y;
            }
        } else {
            // scale_value_float 4 == FIT
            scale.x = scale_value_float == 4 ? scale_max.y : std::min(scale_max.y, scale_value_float);
            float size_y = ((float) getTextureRect().width * scale.x) / game_aspect_ratio;
            if (scaling_mode == "AUTO") {
                scale.y = size_y / (float) getTextureRect().height;
                // use integer scaling if aspect ratio is not too divergent
                if (scale.y / scale.x < 1.2f && scale.y / scale.x > 0.8f) {
                    scale.y = scale.x;
                }
            } else if (scaling_mode == "ASPECT") {
                scale.y = size_y / (float) getTextureRect().height;
            } else {
                // integer scaling
                scale.y = scale.x;
            }
        }
    }

    printf("C2DUIVideo::updateScaling: mode: %s, scaling: %f x %f, size: %i x %i, rotation: %i\n",
           scale_value.c_str(), scale.x, scale.y,
           (int) (getSize().x * scale.x), (int) (getSize().y * scale.y), rotation_cfg);

    setOrigin(Origin::Center);
    setPosition(screen.x / 2, screen.y / 2);
    setScale(scale);
    setRotation(rotation);
}

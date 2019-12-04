//
// Created by cpasjuste on 01/12/16.
//

#include "c2dui.h"

using namespace c2d;
using namespace c2dui;

/*
static int p2(int size) {
    int p2 = 1;
    while (p2 < size)
        p2 *= 2;
    return p2;
}
*/

C2DUIVideo::C2DUIVideo(UIMain *gui, void **_pixels, int *_pitch, const c2d::Vector2f &size, Texture::Format format)
        : C2DTexture(size, format) {

    ui = gui;

    //texture = new C2DTexture({p2((int) size.x), p2((int) size.y)}, format);
    //setTexture(texture);
    //setTextureRect({0, 0, (int) size.x, (int) size.y});
    //printf("game: %ix%i, texture: %ix%i\n",
    //       (int) size.x, (int) size.y,
    //       (int) texture->getSize().x, (int) texture->getSize().y);
    printf("game: %ix%i\n", (int) size.x, (int) size.y);

    if (_pixels) {
        lock(nullptr, _pixels, _pitch);
        unlock();
    }
}

C2DUIVideo::~C2DUIVideo() {
    //delete (texture);
}

void C2DUIVideo::updateScaling(bool vertical, bool flip) {

    int rotated = 0;
    float rotation = 0;
    std::string scale_mode = ui->getConfig()->get(Option::Id::ROM_SCALING, true)->getValueString();
    int rotation_cfg = ui->getConfig()->get(Option::Id::ROM_ROTATION, true)->getIndex();

    Vector2f screen = ui->getSize();
    Vector2f scale_max;
    float sx = 1, sy = 1;

#ifndef __PSP2__
    if (vertical) {
        switch (rotation_cfg) {
            case 1: // ON
                rotation = flip ? 90 : 270;
                rotated = 1;
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
        scale_max.x = screen.x / getTextureRect().height;
        scale_max.y = screen.y / getTextureRect().width;
    } else {
        scale_max.x = screen.x / getTextureRect().width;
        scale_max.y = screen.y / getTextureRect().height;
    }

    if (scale_mode == "NONE") {
        // small screens 1x == downscale
        if (scale_max.x < 1 || scale_max.y < 1) {
            sx = sy = rotated ? scale_max.y : scale_max.x;
        }
    } else if (scale_mode == "2X") {
        sx = sy = std::min(scale_max.x, 2.0f);
        if (sy > scale_max.y) {
            sx = sy = std::min(scale_max.y, 2.0f);
        }
    } else if (scale_mode == "3X") {
        sx = sy = std::min(scale_max.x, 3.0f);
        if (sy > scale_max.y) {
            sx = sy = std::min(scale_max.y, 3.0f);
        }
    } else if (scale_mode == "FIT") {
        sx = sy = scale_max.y;
        if (sx > scale_max.x) {
            sx = sy = scale_max.x;
        }
    } else if (scale_mode == "FIT 4:3") {
        if (rotated) {
            sx = scale_max.y;
            float size_y = sx * getTextureRect().width * 1.33f;
            sy = std::min(scale_max.x, size_y / getTextureRect().height);
        } else {
            sy = scale_max.y;
            float size_x = sy * getTextureRect().height * 1.33f;
            sx = std::min(scale_max.x, size_x / getTextureRect().width);
        }
    } else if (scale_mode == "FULL") {
        sx = rotated ? scale_max.y : scale_max.x;
        sy = rotated ? scale_max.x : scale_max.y;
    }

    printf("C2DUIVideo::updateScaling: mode: %s, scaling: %f x %f, size: %i x %i, rotation: %i\n",
           scale_mode.c_str(), sx, sy, (int) (getSize().x * sx), (int) (getSize().y * sy), rotation_cfg);

    setOrigin(Origin::Center);
    setPosition(screen.x / 2, screen.y / 2);
    setScale(sx, sy);
    setRotation(rotation);
}

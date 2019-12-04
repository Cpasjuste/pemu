//
// Created by cpasjuste on 09/12/18.
//

#ifndef PEMU_UI_MPV_TEXTURE_H
#define PEMU_UI_MPV_TEXTURE_H

#include "mpv.h"

class MpvTexture : public c2d::GLTextureBuffer {

public:

    explicit MpvTexture(const c2d::Vector2f &size, Mpv *mpv);

private:

    void onDraw(c2d::Transform &transform, bool draw = true) override;

    c2d::TweenAlpha *tweenAlpha = nullptr;
    Mpv *mpv = nullptr;
};

#endif //PEMU_UI_MPV_TEXTURE_H

//
// Created by cpasjuste on 09/12/18.
//

#ifndef PEMU_UI_MPV_TEXTURE_H
#define PEMU_UI_MPV_TEXTURE_H

#ifdef __MPV__

#include "mpv.h"

class MpvTexture : public c2d::GLTextureBuffer {

public:

    explicit MpvTexture(const c2d::Vector2f &size, Mpv *mpv);

private:

    void onDraw(c2d::Transform &transform, bool draw = true) override;

    Mpv *mpv = nullptr;
};

#endif // __MPV__
#endif //PEMU_UI_MPV_TEXTURE_H

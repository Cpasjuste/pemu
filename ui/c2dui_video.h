//
// Created by cpasjuste on 25/11/16.
//

#ifndef C2DUI_VIDEO_H_
#define C2DUI_VIDEO_H_

#include <cstring>

namespace c2dui {

    class C2DUIVideo : public c2d::C2DTexture {

    public:

        C2DUIVideo(UIMain *ui, void **pixels, int *pitch, const c2d::Vector2f &size,
                   c2d::Texture::Format format = c2d::Texture::Format::RGB565);

        ~C2DUIVideo() override;

        virtual void updateScaling(bool vertical = false, bool flip = false);

    private:

        UIMain *ui = nullptr;
        //c2d::Texture *texture = nullptr;
    };
}

#endif //C2DUI_VIDEO_H_

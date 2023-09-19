//
// Created by cpasjuste on 25/11/16.
//

#ifndef PFBN_VIDEO_H
#define PFBN_VIDEO_H

namespace pemu {

    class PFBAVideo : public C2DUIVideo {

    public:

        PFBAVideo(UiMain *ui, uint8_t **pixels, int *pitch, const c2d::Vector2i &size,
                  const c2d::Vector2i &aspect = {4, 3});

        void updateScaling(bool vertical = false, bool flip = false) override;
    };
}

#endif //PFBN_VIDEO_H

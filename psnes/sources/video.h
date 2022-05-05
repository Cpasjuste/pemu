//
// Created by cpasjuste on 25/11/16.
//

#ifndef PSNESVIDEO_H
#define PSNESVIDEO_H

namespace c2dui {

    class PSNESVideo : public C2DUIVideo {

    public:

        PSNESVideo(UiMain *ui, uint8_t **pixels, int *pitch, const c2d::Vector2i &size);

        void updateScaling(bool vertical = false, bool flip = false) override;

    private:

        UiMain *ui;
    };
}

#endif //PSNESVIDEO_H

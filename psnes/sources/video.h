//
// Created by cpasjuste on 25/11/16.
//

#ifndef _PSNESVIDEO_H_
#define _PSNESVIDEO_H_

#include <cstring>

namespace c2dui {

    class PSNESVideo : public C2DUIVideo {

    public:

        PSNESVideo(UiMain *ui, void **pixels, int *pitch, const c2d::Vector2f &size);

        void updateScaling(bool vertical = false, bool flip = false);

    private:

        UiMain *ui;
    };
}

#endif //_PSNESVIDEO_H_

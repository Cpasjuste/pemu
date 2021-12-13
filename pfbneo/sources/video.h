//
// Created by cpasjuste on 25/11/16.
//

#ifndef _PFBAVIDEO_H_
#define _PFBAVIDEO_H_

#include <cstring>

namespace c2dui {

    class PFBAVideo : public C2DUIVideo {

    public:

        PFBAVideo(UiMain *ui, void **pixels, int *pitch, const c2d::Vector2f &size);

        void updateScaling(bool vertical = false, bool flip = false);
    };
}

#endif //_PFBAVIDEO_H_

//
// Created by cpasjuste on 25/11/16.
//

#ifndef _PSNESVIDEO_H_
#define _PSNESVIDEO_H_

#include <cstring>

namespace c2dui {

    class PSNESVideo : public C2DUIVideo {

    public:

        PSNESVideo(C2DUIGuiMain *ui, void **pixels, int *pitch, const c2d::Vector2f &size);

        void updateScaling();
//#ifdef __SWITCH__
//        void unlock();
//#endif

    private:

        C2DUIGuiMain *ui;
    };
}

#endif //_PSNESVIDEO_H_

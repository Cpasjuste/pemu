//
// Created by cpasjuste on 01/02/19.
//

#ifndef C2DUI_HIGHLIGHT_H
#define C2DUI_HIGHLIGHT_H

#include "cross2d/skeleton/sfml/RoundedRectangleShape.h"

namespace c2dui {

    class UIHighlight : public c2d::RoundedRectangleShape {

    public:

        explicit UIHighlight();

        void tweenPosition(const c2d::FloatRect &rect, int ms);

        void tweenPosition(c2d::RectangleShape *shape, int ms);

        c2d::Vector2i currentPosition;
        c2d::TweenPosition *tweenPos;
        c2d::TweenSize *tweenSize;
    };
}

#endif //C2DUI_HIGHLIGHT_H

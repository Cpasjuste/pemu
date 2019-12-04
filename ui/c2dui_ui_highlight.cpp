//
// Created by cpasjuste on 01/02/19.
//

#include "c2dui.h"

using namespace c2d;
using namespace c2dui;

UIHighlight::UIHighlight() : RoundedRectangleShape() {

    tweenPos = new TweenPosition({0, 0}, {0, 0}, 0.25f);
    add(tweenPos);
    tweenSize = new TweenSize({0, 0}, {0, 0}, 0.25f);
    add(tweenSize);
}

void UIHighlight::tweenPosition(const c2d::FloatRect &rect, int ms) {
    Vector2i newPos = {(int) rect.left, (int) rect.top};
    if (currentPosition.x != newPos.x || currentPosition.y != newPos.y) {
        // pos
        currentPosition = newPos;
        tweenPos->setFromTo(getPosition(), {(float) newPos.x, (float) newPos.y}, (float) ms * 0.001f);
        tweenPos->play(TweenDirection::Forward, true);
        // size
        tweenSize->setFromTo(getSize(), {rect.width, rect.height}, (float) ms * 0.001f);
        tweenSize->play(TweenDirection::Forward, true);
    }
}

void UIHighlight::tweenPosition(c2d::RectangleShape *shape, int ms) {
    FloatRect bounds = shape->getGlobalBounds();
    if (currentPosition.x != (int) bounds.left || currentPosition.y != (int) bounds.top) {
        // pos
        currentPosition = {(int) bounds.left, (int) bounds.top};
        tweenPos->setFromTo(getPosition(), {bounds.left, bounds.top}, (float) ms * 0.001f);
        tweenPos->play(TweenDirection::Forward, true);
        // size
        tweenSize->setFromTo(getSize(), shape->getSize(), (float) ms * 0.001f);
        tweenSize->play(TweenDirection::Forward, true);
    }
}

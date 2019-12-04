//
// Created by cpasjuste on 04/02/18.
//

#ifndef C2D_UI_PROGRESSBOX_H
#define C2D_UI_PROGRESSBOX_H

namespace c2dui {

    class UIProgressBox : public c2d::RectangleShape {

    public:

        UIProgressBox(UIMain *gui);

        void setTitle(std::string title);

        void setMessage(std::string message);

        void setProgress(float progress);

    private:

        c2d::Text *title;
        c2d::Text *message;
        c2d::RectangleShape *progress_bg;
        c2d::RectangleShape *progress_fg;
    };
}

#endif //C2D_UI_PROGRESSBOX_H

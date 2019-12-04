//
// Created by cpasjuste on 03/02/18.
//

#ifndef C2DUI_UI_EMU_H
#define C2DUI_UI_EMU_H

#include <sys/time.h>

namespace c2dui {

    class UIEmu : public c2d::RectangleShape {

    public:

        UIEmu(UIMain *ui);

        virtual int load(const ss_api::Game &game);

        virtual void stop();

        virtual void pause();

        virtual void resume();

        UIMain *getUi();

        C2DUIVideo *getVideo();

        c2d::Audio *getAudio();

        void addAudio(c2d::Audio *audio);

        void addAudio(int rate = 48000, float fps = 60, c2d::Audio::C2DAudioCallback cb = nullptr);

        void addVideo(C2DUIVideo *video);

        void addVideo(UIMain *ui, void **pixels, int *pitch, const c2d::Vector2f &size,
                      c2d::Texture::Format format = c2d::Texture::Format::RGB565);

        float getFrameDuration();

        void setFrameDuration(float f);

        c2d::Text *getFpsText();

        char *getFpsString();

        bool isPaused();

        bool onInput(c2d::Input::Player *players) override;

    private:

        UIMain *ui = nullptr;
        C2DUIVideo *video = nullptr;
        c2d::Audio *audio = nullptr;
        c2d::Text *fpsText = nullptr;
        char fpsString[32];
        bool paused = true;
        float frameDuration = 0;
    };
}

#endif //C2DUI_UI_EMU_H

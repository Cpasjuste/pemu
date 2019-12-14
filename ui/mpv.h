//
// Created by cpasjuste on 02/04/19.
//

#ifndef PPLAY_MPV_H
#define PPLAY_MPV_H

#ifdef __MPV__

#include <string>
#include <mpv/client.h>
#include <mpv/render_gl.h>

class Mpv {

public:

    enum class LoadType {
        Replace,
        Append,
        AppendPlay
    };

    explicit Mpv(const std::string &configPath, bool initRender);

    ~Mpv();

    int load(const std::string &file, LoadType loadType, const std::string &options);

    int pause();

    int resume();

    int stop();

    int seek(double position);

    int setSpeed(double speed);

    double getSpeed();

    int setVid(int id);

    int setAid(int id);

    int setSid(int id);

    int getVideoBitrate();

    int getAudioBitrate();

    long getDuration();

    long getPosition();

    bool isStopped();

    bool isPaused();

    bool isAvailable();

    mpv_event *getEvent();

    mpv_handle *getHandle();

    mpv_render_context *getContext();

private:

    mpv_handle *handle = nullptr;
    mpv_render_context *context = nullptr;
};

#endif // __MPV__
#endif //PPLAY_MPV_H

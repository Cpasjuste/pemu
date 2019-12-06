//
// Created by cpasjuste on 02/04/19.
//
#ifdef __MPV__

#include <SDL2/SDL_video.h>
#include "mpv.h"

static void *get_proc_address_mpv(void *unused, const char *name) {
    return SDL_GL_GetProcAddress(name);
}

Mpv::Mpv(const std::string &configPath, bool initRender) {

	printf("mpv_create\n");
    handle = mpv_create();
    if (handle == nullptr) {
        printf("Mpv::Mpv: error: mpv_create\n");
        return;
    }
	printf("mpv_set_option_string\n");
    mpv_set_option_string(handle, "config-dir", configPath.c_str());
    mpv_set_option_string(handle, "terminal", "yes");
    mpv_set_option_string(handle, "msg-level", "all=v");
    mpv_set_option_string(handle, "vd-lavc-threads", "4");
    mpv_set_option_string(handle, "vd-lavc-fast", "yes");
    mpv_set_option_string(handle, "vd-lavc-skiploopfilter", "all");
    mpv_set_option_string(handle, "audio-channels", "stereo");
    if (!initRender) {
        mpv_set_option_string(handle, "vid", "no");
        mpv_set_option_string(handle, "aid", "no");
        mpv_set_option_string(handle, "sid", "no");
        mpv_set_option_string(handle, "vo", "null");
        mpv_set_option_string(handle, "ao", "null");
    }
	printf("mpv_initialize\n");
    int res = mpv_initialize(handle);
    if (res != 0) {
        printf("Mpv::Mpv: error: mpv_initialize: %s\n", mpv_error_string(res));
        mpv_terminate_destroy(handle);
        handle = nullptr;
        return;
    }

    if (initRender) {
        mpv_opengl_init_params gl_init_params{get_proc_address_mpv, nullptr, nullptr};
        mpv_render_param params[]{
                {MPV_RENDER_PARAM_API_TYPE,           const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
                {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
                {MPV_RENDER_PARAM_INVALID,            nullptr}
        };

        if (mpv_render_context_create(&context, handle, params) < 0) {
            printf("error: mpv_render_context_create: %s\n", mpv_error_string(res));
            mpv_terminate_destroy(handle);
            handle = nullptr;
        }
    }
}

Mpv::~Mpv() {
    if (context != nullptr) {
        mpv_render_context_free(context);
    }
    if (handle != nullptr) {
        mpv_terminate_destroy(handle);
    }
}

int Mpv::load(const std::string &file, LoadType loadType, const std::string &options) {

    if (handle != nullptr) {
        std::string type = "replace";
        if (loadType == LoadType::Append) {
            type = "append";
        } else if (loadType == LoadType::AppendPlay) {
            type = "append-play";
        }
        const char *cmd[] = {"loadfile", file.c_str(), type.c_str(), options.c_str(), nullptr};
        return mpv_command(handle, cmd);
    }

    return -1;
}

int Mpv::pause() {
    return mpv_command_string(handle, "set pause yes");
}

int Mpv::resume() {
    return mpv_command_string(handle, "set pause no");
}

int Mpv::stop() {
    return mpv_command_string(handle, "stop");
}

int Mpv::seek(double position) {
    std::string cmd = "no-osd seek " + std::to_string(position) + " absolute";
    return mpv_command_string(handle, cmd.c_str());
}

int Mpv::setSpeed(double speed) {
    std::string cmd = "set speed " + std::to_string(speed);
    return mpv_command_string(handle, cmd.c_str());
}

double Mpv::getSpeed() {
    double res = -1;
    mpv_get_property(handle, "speed", MPV_FORMAT_DOUBLE, &res);
    return res;
}

int Mpv::setVid(int id) {
    if (id > -1) {
        std::string cmd = "no-osd set vid " + std::to_string(id);
        return mpv_command_string(handle, cmd.c_str());
    }
    return -1;
}

int Mpv::setAid(int id) {
    if (id > -1) {
        std::string cmd = "no-osd set aid " + std::to_string(id);
        return mpv_command_string(handle, cmd.c_str());
    }
    return -1;
}

int Mpv::setSid(int id) {
    std::string cmd = "no-osd set sid ";
    cmd += id < 0 ? "no" : std::to_string(id);
    return mpv_command_string(handle, cmd.c_str());
}

int Mpv::getVideoBitrate() {
    double bitrate = 0;
    mpv_get_property(handle, "video-bitrate", MPV_FORMAT_DOUBLE, &bitrate);
    return (int) bitrate;
}

int Mpv::getAudioBitrate() {
    double bitrate = 0;
    mpv_get_property(handle, "audio-bitrate", MPV_FORMAT_INT64, &bitrate);
    return (int) bitrate;
}

long Mpv::getDuration() {
    long duration = 0;
    mpv_get_property(handle, "duration", MPV_FORMAT_INT64, &duration);
    return duration;
}

long Mpv::getPosition() {
    long position = 0;
    mpv_get_property(handle, "playback-time", MPV_FORMAT_INT64, &position);
    return position;
}

bool Mpv::isAvailable() {
    return handle != nullptr;
}

bool Mpv::isStopped() {
    int res = 1;
    mpv_get_property(handle, "playback-abort", MPV_FORMAT_FLAG, &res);
    return res == 1;
}

bool Mpv::isPaused() {
    int res = -1;
    mpv_get_property(handle, "pause", MPV_FORMAT_FLAG, &res);
    return res == 1;
}

mpv_event *Mpv::getEvent() {
    return mpv_wait_event(handle, 0);
}

mpv_handle *Mpv::getHandle() {
    return handle;
}

mpv_render_context *Mpv::getContext() {
    return context;
}

#endif // __MPV__

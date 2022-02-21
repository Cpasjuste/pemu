//
// Created by cpasjuste on 09/12/18.
//

#ifdef __MPV__

#include "cross2d/c2d.h"
#include "mpv_texture.h"

using namespace c2d;

MpvTexture::MpvTexture(const c2d::Vector2f &size, Mpv *m) : GLTextureBuffer(size, Format::RGBA8) {
    mpv = m;
}

void MpvTexture::onDraw(c2d::Transform &transform, bool draw) {
    if (draw && mpv && mpv->isAvailable()) {
        bool update = mpv_render_context_update(mpv->getContext()) & MPV_RENDER_UPDATE_FRAME;
        if (update) {
            int zero{0};
            mpv_opengl_fbo mpv_fbo{
                    .fbo = (int) fbo,
                    .w = (int) getSize().x, .h = (int) getSize().y,
                    .internal_format = GL_RGBA8};
            mpv_render_param r_params[] = {
                    {MPV_RENDER_PARAM_OPENGL_FBO, &mpv_fbo},
                    {MPV_RENDER_PARAM_FLIP_Y,     &zero},
                    {MPV_RENDER_PARAM_INVALID,    nullptr}
            };

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
            mpv_render_context_render(mpv->getContext(), r_params);
            glViewport(vp[0], vp[1], (GLsizei) vp[2], (GLsizei) vp[3]);
        }
    }

    GLTextureBuffer::onDraw(transform, draw);
}

#endif // __MPV__
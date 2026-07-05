#include "lf_mpv_gl_preview.h"

#include "common/lf_mpv_session.h"

#include <mpv/render.h>
#include <mpv/render_gl.h>

namespace lf {

namespace {

void* gl_proc_address_resolver(void* ctx, const char* name) {
    const auto* resolver = static_cast<const GlProcResolver*>(ctx);
    if (!resolver || !*resolver) {
        return nullptr;
    }
    return (*resolver)(name);
}

struct GlPreviewState {
    MpvSession*         session   = nullptr;
    mpv_render_context* render    = nullptr;
    GlProcResolver      get_proc;
    RedrawCallback      on_redraw;
};

void on_render_update(void* ctx) {
    auto* state = static_cast<GlPreviewState*>(ctx);
    if (state && state->on_redraw) {
        state->on_redraw();
    }
}

}  // namespace

struct MpvGlPreview::Impl {
    GlPreviewState state;

    explicit Impl(MpvSession& session_ref)
        : state{} {
        state.session = &session_ref;
    }
};

MpvGlPreview::MpvGlPreview(MpvSession& session)
    : impl_(new Impl(session)) {}

MpvGlPreview::~MpvGlPreview() {
    detach_gl();
    delete impl_;
}

bool MpvGlPreview::attach_gl(GlProcResolver get_proc) {
    if (!impl_->state.session || !impl_->state.session->mpv || !get_proc) {
        return false;
    }
    if (impl_->state.render) {
        return true;
    }

    impl_->state.get_proc = std::move(get_proc);

    mpv_opengl_init_params gl_init{
        gl_proc_address_resolver,
        &impl_->state.get_proc,
    };

    mpv_render_param params[] = {
        {MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL)},
        {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init},
        {MPV_RENDER_PARAM_INVALID, nullptr},
    };

    const int status = mpv_render_context_create(
        &impl_->state.render,
        impl_->state.session->mpv,
        params);
    if (status < 0 || !impl_->state.render) {
        return false;
    }

    mpv_render_context_set_update_callback(
        impl_->state.render,
        on_render_update,
        &impl_->state);
    return true;
}

void MpvGlPreview::detach_gl() {
    if (impl_->state.render) {
        mpv_render_context_free(impl_->state.render);
        impl_->state.render = nullptr;
    }
}

bool MpvGlPreview::render(unsigned int fbo, int width, int height) {
    if (!impl_->state.render || width <= 0 || height <= 0) {
        return false;
    }

    mpv_opengl_fbo target{
        static_cast<int>(fbo),
        width,
        height,
        0,
    };

    int flip_y = 1;

    mpv_render_param params[] = {
        {MPV_RENDER_PARAM_OPENGL_FBO, &target},
        {MPV_RENDER_PARAM_FLIP_Y, &flip_y},
        {MPV_RENDER_PARAM_INVALID, nullptr},
    };

    const int status = mpv_render_context_render(impl_->state.render, params);
    return status >= 0;
}

void MpvGlPreview::set_redraw_callback(RedrawCallback callback) {
    impl_->state.on_redraw                 = std::move(callback);
    impl_->state.session->on_wakeup = impl_->state.on_redraw;
}

}  // namespace lf

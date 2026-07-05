#pragma once

#include "render/abstract/lf_preview_renderer.h"

struct mpv_render_context;

namespace lf {

struct MpvSession;

class MpvGlPreview : public PreviewRenderer {
public:
    explicit MpvGlPreview(MpvSession& session);
    ~MpvGlPreview() override;

    bool attach_gl(GlProcResolver get_proc) override;
    void detach_gl() override;
    bool render(unsigned int fbo, int width, int height) override;
    void set_redraw_callback(RedrawCallback callback) override;

private:
    struct Impl;
    Impl* impl_;
};

}  // namespace lf

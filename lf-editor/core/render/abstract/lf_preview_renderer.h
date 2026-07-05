#pragma once

#include <functional>

namespace lf {

using GlProcResolver = std::function<void*(const char* name)>;
using RedrawCallback = std::function<void()>;

class PreviewRenderer {
public:
    virtual ~PreviewRenderer() = default;

    virtual bool attach_gl(GlProcResolver get_proc) = 0;
    virtual void detach_gl()                        = 0;
    virtual bool render(unsigned int fbo, int width, int height) = 0;
    virtual void set_redraw_callback(RedrawCallback callback)    = 0;
};

}  // namespace lf

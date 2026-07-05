#include "render/lf_video_surface.h"

namespace lf {

VideoSurface::VideoSurface() = default;
VideoSurface::~VideoSurface() = default;

void VideoSurface::resize(int width, int height) {
    if (width <= 0 || height <= 0) {
        width_  = 0;
        height_ = 0;
        buffer_.clear();
        return;
    }
    width_  = width;
    height_ = height;
    buffer_.assign(static_cast<std::size_t>(width * height * 4), 0);
}

void VideoSurface::upload_frame(const RenderFrame& frame) {
    if (frame.pixels && frame.width > 0 && frame.height > 0) {
        resize(frame.width, frame.height);
    }
}

int VideoSurface::width() const {
    return width_;
}

int VideoSurface::height() const {
    return height_;
}

const std::uint8_t* VideoSurface::pixels() const {
    return buffer_.empty() ? nullptr : buffer_.data();
}

}  // namespace lf

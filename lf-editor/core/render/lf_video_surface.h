#pragma once

#include <cstdint>
#include <vector>

namespace lf {

struct RenderFrame {
    void*  pixels       = nullptr;
    int    width        = 0;
    int    height       = 0;
    int    stride       = 0;
    double pts_seconds  = 0.0;
};

class VideoSurface {
public:
    VideoSurface();
    ~VideoSurface();

    void resize(int width, int height);
    void upload_frame(const RenderFrame& frame);

    int width() const;
    int height() const;

    const std::uint8_t* pixels() const;

private:
    int width_  = 0;
    int height_ = 0;
    std::vector<std::uint8_t> buffer_;
};

}  // namespace lf

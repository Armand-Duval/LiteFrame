#pragma once

#include <string>

namespace lf {

class MediaPlayer {
public:
    virtual ~MediaPlayer() = default;

    virtual bool open(const std::string& path)  = 0;
    virtual void close()                        = 0;
    virtual bool play()                         = 0;
    virtual bool pause()                        = 0;
    virtual bool seek(double seconds)           = 0;

    virtual bool   is_playing() const           = 0;
    virtual bool   has_media() const            = 0;
    virtual double position_seconds() const     = 0;
    virtual double duration_seconds() const     = 0;

    virtual void poll() = 0;
};

}  // namespace lf

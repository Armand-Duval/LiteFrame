#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace lf {

struct TimelineClip {
    std::string source_path;
    double      in_point  = 0.0;
    double      out_point = 0.0;
    double      start     = 0.0;
};

class OtioTimeline {
public:
    OtioTimeline();
    ~OtioTimeline();

    void clear();
    void add_clip(TimelineClip clip);

    double duration_seconds() const;
    std::size_t clip_count() const;

    const TimelineClip& clip_at(std::size_t index) const;

    bool load_from_file(const std::string& path);
    bool save_to_file(const std::string& path) const;

private:
    std::vector<TimelineClip> clips_;
};

}  // namespace lf

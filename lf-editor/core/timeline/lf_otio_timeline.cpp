#include "timeline/lf_otio_timeline.h"

namespace lf {

OtioTimeline::OtioTimeline() = default;
OtioTimeline::~OtioTimeline() = default;

void OtioTimeline::clear() {
    clips_.clear();
}

void OtioTimeline::add_clip(TimelineClip clip) {
    clips_.push_back(std::move(clip));
}

double OtioTimeline::duration_seconds() const {
    double end = 0.0;
    for (const auto& clip : clips_) {
        const double clip_end = clip.start + (clip.out_point - clip.in_point);
        if (clip_end > end) {
            end = clip_end;
        }
    }
    return end;
}

std::size_t OtioTimeline::clip_count() const {
    return clips_.size();
}

const TimelineClip& OtioTimeline::clip_at(std::size_t index) const {
    return clips_.at(index);
}

bool OtioTimeline::load_from_file(const std::string& path) {
    (void)path;
    return false;
}

bool OtioTimeline::save_to_file(const std::string& path) const {
    (void)path;
    return false;
}

}  // namespace lf

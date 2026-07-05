#pragma once

#include "timeline/lf_otio_timeline.h"

#include <string>

namespace lf {

struct ExportSettings {
    std::string output_path;
    int         width  = 1920;
    int         height = 1080;
    int         fps    = 30;
};

class FfmpegExporter {
public:
    FfmpegExporter();
    ~FfmpegExporter();

    bool export_timeline(const OtioTimeline& timeline, const ExportSettings& settings);

private:
    struct Impl;
    Impl* impl_;
};

}  // namespace lf

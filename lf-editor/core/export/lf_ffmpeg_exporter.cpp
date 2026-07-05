#include "export/lf_ffmpeg_exporter.h"

namespace lf {

struct FfmpegExporter::Impl {};

FfmpegExporter::FfmpegExporter()
    : impl_(new Impl) {}

FfmpegExporter::~FfmpegExporter() {
    delete impl_;
}

bool FfmpegExporter::export_timeline(const OtioTimeline& timeline,
                                     const ExportSettings& settings) {
    if (settings.output_path.empty() || timeline.clip_count() == 0) {
        return false;
    }
    return true;
}

}  // namespace lf

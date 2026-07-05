#include "lf_editor.h"

#include "export/lf_ffmpeg_exporter.h"
#include "player/abstract/lf_media_player.h"
#include "player/lf_mpv_media_player.h"
#include "common/lf_mpv_session.h"
#include "render/abstract/lf_preview_renderer.h"
#include "render/lf_mpv_gl_preview.h"
#include "render/lf_video_surface.h"
#include "timeline/lf_otio_timeline.h"

namespace lf {

struct Editor::Impl {
    EditorConfig config;
    std::unique_ptr<MpvSession>      session;
    std::unique_ptr<MpvMediaPlayer>  playback;
    std::unique_ptr<MpvGlPreview>    preview;
    std::unique_ptr<VideoSurface>    surface;
    std::unique_ptr<OtioTimeline>    timeline;
    std::unique_ptr<FfmpegExporter>  exporter;

    explicit Impl(EditorConfig config_in)
        : config(std::move(config_in)),
          session(std::make_unique<MpvSession>()),
          playback(std::make_unique<MpvMediaPlayer>(*session)),
          preview(std::make_unique<MpvGlPreview>(*session)),
          surface(std::make_unique<VideoSurface>()),
          timeline(std::make_unique<OtioTimeline>()),
          exporter(std::make_unique<FfmpegExporter>()) {
        surface->resize(config.preview_width, config.preview_height);
    }
};

Editor::Transport::Transport(Impl* impl)
    : impl_(impl) {}

Editor::Preview::Preview(Impl* impl)
    : impl_(impl) {}

Editor::Editor(EditorConfig config)
    : impl_(std::make_unique<Impl>(std::move(config))),
      transport_(impl_.get()),
      preview_(impl_.get()) {}

Editor::~Editor() = default;

bool Editor::load_media(const std::string& path) {
    if (!impl_->playback->open(path)) {
        return false;
    }
    impl_->timeline->clear();
    impl_->timeline->add_clip({path, 0.0, impl_->playback->duration_seconds(), 0.0});
    return true;
}

bool Editor::export_timeline(const std::string& output_path) {
    ExportSettings settings;
    settings.output_path = output_path;
    settings.width       = impl_->config.preview_width;
    settings.height      = impl_->config.preview_height;
    return impl_->exporter->export_timeline(*impl_->timeline, settings);
}

Editor::Transport& Editor::transport() { return transport_; }
Editor::Preview& Editor::preview() { return preview_; }
const Editor::Transport& Editor::transport() const { return transport_; }
const Editor::Preview& Editor::preview() const { return preview_; }

void Editor::Transport::play() { impl_->playback->play(); }
void Editor::Transport::pause() { impl_->playback->pause(); }
void Editor::Transport::seek(double seconds) { impl_->playback->seek(seconds); }
double Editor::Transport::position_seconds() const {
    return impl_->playback->position_seconds();
}
double Editor::Transport::duration_seconds() const {
    return impl_->playback->duration_seconds();
}
void Editor::Transport::poll() { impl_->playback->poll(); }

void Editor::Preview::set_redraw_callback(std::function<void()> callback) {
    impl_->preview->set_redraw_callback(std::move(callback));
}

bool Editor::Preview::attach_gl(std::function<void*(const char* name)> get_proc) {
    return impl_->preview->attach_gl(std::move(get_proc));
}

void Editor::Preview::detach_gl() {
    impl_->preview->detach_gl();
    impl_->preview->set_redraw_callback({});
}

bool Editor::Preview::render(unsigned int fbo, int width, int height) {
    return impl_->preview->render(fbo, width, height);
}

}  // namespace lf

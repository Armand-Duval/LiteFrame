#pragma once

#include <functional>
#include <memory>
#include <string>

namespace lf {

struct EditorConfig {
    std::string media_path;
    int preview_width  = 1280;
    int preview_height = 720;
};

class Editor {
public:
    explicit Editor(EditorConfig config = {});
    ~Editor();

    Editor(const Editor&)            = delete;
    Editor& operator=(const Editor&) = delete;

    bool load_media(const std::string& path);
    bool export_timeline(const std::string& output_path);

    void play();
    void pause();
    void seek(double seconds);
    double position_seconds() const;
    double duration_seconds() const;

    void poll();

    void set_preview_redraw_callback(std::function<void()> callback);
    bool attach_preview_gl(std::function<void*(const char* name)> get_proc);
    void detach_preview_gl();
    bool render_preview(unsigned int fbo, int width, int height);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace lf

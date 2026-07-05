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
    struct Impl;

public:
    class Transport {
    public:
        void play();
        void pause();
        void seek(double seconds);
        double position_seconds() const;
        double duration_seconds() const;
        void poll();

    private:
        friend class Editor;
        explicit Transport(Impl* impl);

        Impl* impl_ = nullptr;
    };

    class Preview {
    public:
        void set_redraw_callback(std::function<void()> callback);
        bool attach_gl(std::function<void*(const char* name)> get_proc);
        void detach_gl();
        bool render(unsigned int fbo, int width, int height);

    private:
        friend class Editor;
        explicit Preview(Impl* impl);

        Impl* impl_ = nullptr;
    };

    explicit Editor(EditorConfig config = {});
    ~Editor();

    Editor(const Editor&)            = delete;
    Editor& operator=(const Editor&) = delete;

    bool load_media(const std::string& path);
    bool export_timeline(const std::string& output_path);

    Transport&       transport();
    Preview&         preview();
    const Transport& transport() const;
    const Preview&   preview() const;

private:
    std::unique_ptr<Impl> impl_;
    Transport             transport_;
    Preview               preview_;
};

}  // namespace lf

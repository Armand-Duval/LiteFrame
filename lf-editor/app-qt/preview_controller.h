#pragma once

#include <QObject>
#include <functional>

namespace lf {
class Editor;
}

class PreviewController : public QObject {
    Q_OBJECT

public:
    explicit PreviewController(lf::Editor* editor, QObject* parent = nullptr);

    void set_redraw_callback(std::function<void()> callback);
    bool attach_gl(std::function<void*(const char* name)> get_proc);
    void detach_gl();
    bool render(unsigned int fbo, int width, int height);

private:
    lf::Editor* editor_ = nullptr;
};

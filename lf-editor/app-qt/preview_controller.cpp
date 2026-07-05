#include "preview_controller.h"

#include "lf_editor.h"

PreviewController::PreviewController(lf::Editor* editor, QObject* parent)
    : QObject(parent),
      editor_(editor) {}

void PreviewController::set_redraw_callback(std::function<void()> callback) {
    if (!editor_) {
        return;
    }
    editor_->preview().set_redraw_callback(std::move(callback));
}

bool PreviewController::attach_gl(std::function<void*(const char* name)> get_proc) {
    if (!editor_) {
        return false;
    }
    return editor_->preview().attach_gl(std::move(get_proc));
}

void PreviewController::detach_gl() {
    if (!editor_) {
        return;
    }
    editor_->preview().detach_gl();
    editor_->preview().set_redraw_callback({});
}

bool PreviewController::render(unsigned int fbo, int width, int height) {
    if (!editor_) {
        return false;
    }
    return editor_->preview().render(fbo, width, height);
}

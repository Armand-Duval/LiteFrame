#include "preview_item.h"

#include "lf_editor.h"

#include <QMetaObject>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>

class PreviewGlRenderer : public QQuickFramebufferObject::Renderer {
public:
    QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject* item) override {
        auto* preview = static_cast<PreviewItem*>(item);

        if (editor_ != preview->editor_) {
            editor_   = preview->editor_;
            gl_ready_ = false;
        }

        has_media_ = preview->has_media_;
    }

    void render() override {
        if (!editor_ || !has_media_) {
            return;
        }

        if (!gl_ready_) {
            auto* context = QOpenGLContext::currentContext();
            if (!context) {
                return;
            }

            gl_ready_ = editor_->attach_preview_gl([context](const char* name) -> void* {
                return reinterpret_cast<void*>(context->getProcAddress(name));
            });
        }

        if (!gl_ready_) {
            return;
        }

        QOpenGLFramebufferObject* fbo = framebufferObject();
        if (!fbo) {
            return;
        }

        editor_->render_preview(
            static_cast<unsigned int>(fbo->handle()),
            fbo->width(),
            fbo->height());
    }

private:
    lf::Editor* editor_    = nullptr;
    bool        has_media_ = false;
    bool        gl_ready_  = false;
};

PreviewItem::PreviewItem(QQuickItem* parent)
    : QQuickFramebufferObject(parent) {
    setMirrorVertically(true);
    setImplicitSize(640, 360);
}

PreviewItem::~PreviewItem() {
    if (editor_) {
        editor_->detach_preview_gl();
    }
}

void PreviewItem::bindEditor(lf::Editor* editor) {
    if (editor_ == editor) {
        return;
    }

    if (editor_) {
        editor_->detach_preview_gl();
    }

    editor_ = editor;

    if (editor_) {
        editor_->set_preview_redraw_callback([this]() { requestRedraw(); });
    }

    update();
}

bool PreviewItem::hasMedia() const {
    return has_media_;
}

void PreviewItem::setHasMedia(bool hasMedia) {
    if (has_media_ == hasMedia) {
        return;
    }
    has_media_ = hasMedia;
    emit hasMediaChanged();
    update();
}

QQuickFramebufferObject::Renderer* PreviewItem::createRenderer() const {
    return new PreviewGlRenderer();
}

void PreviewItem::requestRedraw() {
    QMetaObject::invokeMethod(
        this,
        [this]() { update(); },
        Qt::QueuedConnection);
}

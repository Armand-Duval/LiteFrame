#include "preview_item.h"

#include "preview_controller.h"

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

        if (controller_ != preview->controller_) {
            controller_ = preview->controller_;
            gl_ready_   = false;
        }

        has_media_ = preview->has_media_;
    }

    void render() override {
        if (!controller_ || !has_media_) {
            return;
        }

        if (!gl_ready_) {
            auto* context = QOpenGLContext::currentContext();
            if (!context) {
                return;
            }

            gl_ready_ = controller_->attach_gl([context](const char* name) -> void* {
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

        controller_->render(
            static_cast<unsigned int>(fbo->handle()),
            fbo->width(),
            fbo->height());
    }

private:
    PreviewController* controller_ = nullptr;
    bool               has_media_  = false;
    bool               gl_ready_   = false;
};

PreviewItem::PreviewItem(QQuickItem* parent)
    : QQuickFramebufferObject(parent) {
    setMirrorVertically(true);
    setImplicitSize(640, 360);
}

PreviewItem::~PreviewItem() {
    if (controller_) {
        controller_->detach_gl();
    }
}

void PreviewItem::bindPreviewController(PreviewController* controller) {
    if (controller_ == controller) {
        return;
    }

    if (controller_) {
        controller_->detach_gl();
    }

    controller_ = controller;

    if (controller_) {
        controller_->set_redraw_callback([this]() { requestRedraw(); });
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

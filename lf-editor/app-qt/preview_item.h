#pragma once

#include <QQuickFramebufferObject>
#include <QtQml/qqmlregistration.h>

namespace lf {
class Editor;
}

class PreviewItem : public QQuickFramebufferObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(PreviewViewport)
    Q_PROPERTY(bool hasMedia READ hasMedia WRITE setHasMedia NOTIFY hasMediaChanged)

public:
    explicit PreviewItem(QQuickItem* parent = nullptr);
    ~PreviewItem() override;

    void bindEditor(lf::Editor* editor);

    bool hasMedia() const;
    void setHasMedia(bool hasMedia);

signals:
    void hasMediaChanged();

protected:
    QQuickFramebufferObject::Renderer* createRenderer() const override;

private:
    friend class PreviewGlRenderer;

    void requestRedraw();

    lf::Editor* editor_    = nullptr;
    bool        has_media_ = false;
};

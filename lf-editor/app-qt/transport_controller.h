#pragma once

#include <QObject>
#include <QTimer>
#include <string>

namespace lf {
class Editor;
}

class TransportController : public QObject {
    Q_OBJECT

    Q_PROPERTY(double position READ position NOTIFY positionChanged)
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(bool hasMedia READ hasMedia NOTIFY hasMediaChanged)
    Q_PROPERTY(QString mediaPath READ mediaPath NOTIFY mediaPathChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(bool seeking READ seeking WRITE setSeeking NOTIFY seekingChanged)

public:
    explicit TransportController(lf::Editor* editor, QObject* parent = nullptr);
    ~TransportController() override;

    double  position() const;
    double  duration() const;
    bool    hasMedia() const;
    QString mediaPath() const;
    QString statusMessage() const;
    bool    seeking() const;

    void setSeeking(bool seeking);

    Q_INVOKABLE bool openMedia(const QString& path);
    Q_INVOKABLE bool exportTimeline(const QString& outputPath);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void seek(double seconds);

signals:
    void positionChanged();
    void durationChanged();
    void hasMediaChanged();
    void mediaPathChanged();
    void statusMessageChanged();
    void seekingChanged();
    void openFailed(const QString& message);
    void exportFailed(const QString& message);

private slots:
    void pollTransport();

private:
    void refreshTransportState();
    void setStatusMessage(const QString& message);

    lf::Editor* editor_     = nullptr;
    QTimer*     poll_timer_ = nullptr;

    QString media_path_;
    QString status_message_;
    double  last_position_ = -1.0;
    double  last_duration_ = -1.0;
    bool    has_media_     = false;
    bool    seeking_       = false;
};

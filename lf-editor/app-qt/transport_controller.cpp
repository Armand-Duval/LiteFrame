#include "transport_controller.h"

#include "lf_editor.h"

#include <QFileInfo>
#include <QUrl>
#include <cmath>

TransportController::TransportController(lf::Editor* editor, QObject* parent)
    : QObject(parent),
      editor_(editor) {
    poll_timer_ = new QTimer(this);
    poll_timer_->setInterval(16);
    connect(poll_timer_, &QTimer::timeout, this, &TransportController::pollTransport);
    poll_timer_->start();
}

TransportController::~TransportController() = default;

double TransportController::position() const {
    return editor_->transport().position_seconds();
}

double TransportController::duration() const {
    return editor_->transport().duration_seconds();
}

bool TransportController::hasMedia() const {
    return has_media_;
}

QString TransportController::mediaPath() const {
    return media_path_;
}

QString TransportController::statusMessage() const {
    return status_message_;
}

bool TransportController::seeking() const {
    return seeking_;
}

void TransportController::setSeeking(bool seeking) {
    if (seeking_ == seeking) {
        return;
    }
    seeking_ = seeking;
    emit seekingChanged();
}

bool TransportController::openMedia(const QString& path) {
    if (path.isEmpty() || !editor_) {
        return false;
    }

    const QUrl url(path);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : path;

    if (!editor_->load_media(localPath.toStdString())) {
        emit openFailed(tr("Could not open media file."));
        setStatusMessage(tr("Open failed"));
        return false;
    }

    has_media_ = true;
    media_path_ = localPath;
    emit hasMediaChanged();
    emit mediaPathChanged();

    refreshTransportState();
    last_position_ = editor_->transport().position_seconds();
    last_duration_ = editor_->transport().duration_seconds();

    setStatusMessage(tr("Opened: %1").arg(QFileInfo(localPath).fileName()));
    return true;
}

bool TransportController::exportTimeline(const QString& outputPath) {
    if (outputPath.isEmpty() || !editor_) {
        return false;
    }

    const QUrl url(outputPath);
    const QString localPath = url.isLocalFile() ? url.toLocalFile() : outputPath;

    if (!editor_->export_timeline(localPath.toStdString())) {
        emit exportFailed(tr("Could not export timeline."));
        setStatusMessage(tr("Export failed"));
        return false;
    }

    setStatusMessage(tr("Exported: %1").arg(QFileInfo(localPath).fileName()));
    return true;
}

void TransportController::play() {
    editor_->transport().play();
}

void TransportController::pause() {
    editor_->transport().pause();
}

void TransportController::seek(double seconds) {
    editor_->transport().seek(seconds);
    last_position_ = editor_->transport().position_seconds();
    emit positionChanged();
}

void TransportController::pollTransport() {
    editor_->transport().poll();

    const double position = editor_->transport().position_seconds();
    const double duration = editor_->transport().duration_seconds();

    if (!seeking_ && !std::isnan(position) && position != last_position_) {
        last_position_ = position;
        emit positionChanged();
    }

    if (!std::isnan(duration) && duration > 0.0 && duration != last_duration_) {
        last_duration_ = duration;
        emit durationChanged();
    }
}

void TransportController::refreshTransportState() {
    emit positionChanged();
    emit durationChanged();
}

void TransportController::setStatusMessage(const QString& message) {
    if (status_message_ == message) {
        return;
    }
    status_message_ = message;
    emit statusMessageChanged();
}

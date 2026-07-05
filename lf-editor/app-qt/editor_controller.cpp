#include "editor_controller.h"

#include "preview_item.h"

#include "lf_editor.h"

#include <QFileInfo>
#include <QUrl>
#include <cmath>

EditorController::EditorController(QObject* parent)
    : QObject(parent),
      editor_(std::make_unique<lf::Editor>()) {
    poll_timer_ = new QTimer(this);
    poll_timer_->setInterval(16);
    connect(poll_timer_, &QTimer::timeout, this, &EditorController::pollPlayer);
    poll_timer_->start();
}

EditorController::~EditorController() = default;

double EditorController::position() const {
    return editor_->position_seconds();
}

double EditorController::duration() const {
    return editor_->duration_seconds();
}

bool EditorController::hasMedia() const {
    return has_media_;
}

QString EditorController::mediaPath() const {
    return media_path_;
}

QString EditorController::statusMessage() const {
    return status_message_;
}

bool EditorController::seeking() const {
    return seeking_;
}

void EditorController::setSeeking(bool seeking) {
    if (seeking_ == seeking) {
        return;
    }
    seeking_ = seeking;
    emit seekingChanged();
}

void EditorController::attachPreview(QObject* preview) {
    preview_ = qobject_cast<PreviewItem*>(preview);
    if (preview_) {
        preview_->bindEditor(editor_.get());
    }
}

bool EditorController::openMedia(const QString& path) {
    if (path.isEmpty()) {
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
    last_position_ = editor_->position_seconds();
    last_duration_ = editor_->duration_seconds();

    if (preview_) {
        preview_->update();
    }

    setStatusMessage(tr("Opened: %1").arg(QFileInfo(localPath).fileName()));
    return true;
}

bool EditorController::exportTimeline(const QString& outputPath) {
    if (outputPath.isEmpty()) {
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

void EditorController::play() {
    editor_->play();
}

void EditorController::pause() {
    editor_->pause();
}

void EditorController::seek(double seconds) {
    editor_->seek(seconds);
    last_position_ = editor_->position_seconds();
    emit positionChanged();

    if (preview_) {
        preview_->update();
    }
}

void EditorController::pollPlayer() {
    editor_->poll();

    const double position = editor_->position_seconds();
    const double duration = editor_->duration_seconds();

    if (!seeking_ && !std::isnan(position) && position != last_position_) {
        last_position_ = position;
        emit positionChanged();
    }

    if (!std::isnan(duration) && duration > 0.0 && duration != last_duration_) {
        last_duration_ = duration;
        emit durationChanged();
    }
}

void EditorController::refreshTransportState() {
    emit positionChanged();
    emit durationChanged();
}

void EditorController::setStatusMessage(const QString& message) {
    if (status_message_ == message) {
        return;
    }
    status_message_ = message;
    emit statusMessageChanged();
}

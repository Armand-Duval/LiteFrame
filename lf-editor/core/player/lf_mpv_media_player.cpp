#include "lf_mpv_media_player.h"

#include "common/lf_mpv_session.h"

#include <mpv/client.h>

#include <string>

namespace lf {

namespace {

void check_mpv_error(int status) {
    if (status < 0) {
        // Errors are surfaced via MPV_EVENT logs; avoid throwing across API boundary.
    }
}

}  // namespace

MpvMediaPlayer::MpvMediaPlayer(MpvSession& session)
    : session_(session) {}

bool MpvMediaPlayer::open(const std::string& path) {
    if (!session_.mpv || path.empty()) {
        return false;
    }

    const char* cmd[] = {"loadfile", path.c_str(), nullptr};
    const int   status = mpv_command(session_.mpv, cmd);
    if (status < 0) {
        return false;
    }

    session_.has_media = true;
    session_.position  = 0.0;
    session_.update_duration();
    session_.update_pause_state();
    return true;
}

void MpvMediaPlayer::close() {
    if (!session_.mpv) {
        return;
    }
    const char* cmd[] = {"stop", nullptr};
    mpv_command(session_.mpv, cmd);
    session_.has_media = false;
    session_.playing   = false;
    session_.position  = 0.0;
    session_.duration  = 0.0;
}

bool MpvMediaPlayer::play() {
    if (!session_.mpv) {
        return false;
    }
    int pause = 0;
    check_mpv_error(mpv_set_property(session_.mpv, "pause", MPV_FORMAT_FLAG, &pause));
    session_.playing = true;
    return true;
}

bool MpvMediaPlayer::pause() {
    if (!session_.mpv) {
        return false;
    }
    int pause = 1;
    check_mpv_error(mpv_set_property(session_.mpv, "pause", MPV_FORMAT_FLAG, &pause));
    session_.playing = false;
    return true;
}

bool MpvMediaPlayer::seek(double seconds) {
    if (!session_.mpv || seconds < 0.0) {
        return false;
    }

    const std::string target = std::to_string(seconds);
    const char*       cmd[]  = {"seek", target.c_str(), "absolute", nullptr};
    const int         status = mpv_command(session_.mpv, cmd);
    if (status < 0) {
        return false;
    }

    session_.position = seconds;
    return true;
}

bool MpvMediaPlayer::is_playing() const {
    return session_.playing;
}

bool MpvMediaPlayer::has_media() const {
    return session_.has_media;
}

double MpvMediaPlayer::position_seconds() const {
    return session_.position;
}

double MpvMediaPlayer::duration_seconds() const {
    return session_.duration;
}

void MpvMediaPlayer::poll() {
    session_.poll_events();
}

}  // namespace lf

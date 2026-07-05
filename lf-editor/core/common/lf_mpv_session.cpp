#include "common/lf_mpv_session.h"

#include <mpv/client.h>

#include <cstring>

namespace lf {

namespace {

void check_mpv_error(int status) {
    if (status < 0) {
        // Errors are surfaced via MPV_EVENT logs; avoid throwing across API boundary.
    }
}

void on_mpv_wakeup(void* ctx) {
    auto* session = static_cast<MpvSession*>(ctx);
    if (session && session->on_wakeup) {
        session->on_wakeup();
    }
}

}  // namespace

MpvSession::MpvSession() {
    mpv = mpv_create();
    if (!mpv) {
        return;
    }

    mpv_set_option_string(mpv, "terminal", "no");
    mpv_set_option_string(mpv, "msg-level", "all=no");
    mpv_set_option_string(mpv, "vo", "libmpv");
    mpv_set_option_string(mpv, "keep-open", "yes");
    mpv_set_option_string(mpv, "idle", "yes");
    mpv_set_option_string(mpv, "pause", "yes");

    mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(mpv, 0, "pause", MPV_FORMAT_FLAG);
    mpv_set_wakeup_callback(mpv, on_mpv_wakeup, this);

    check_mpv_error(mpv_initialize(mpv));
}

MpvSession::~MpvSession() {
    if (mpv) {
        mpv_terminate_destroy(mpv);
        mpv = nullptr;
    }
}

void MpvSession::handle_event(const mpv_event* event) {
    switch (event->event_id) {
    case MPV_EVENT_FILE_LOADED:
        has_media = true;
        update_duration();
        break;
    case MPV_EVENT_END_FILE:
        has_media = false;
        playing   = false;
        break;
    case MPV_EVENT_PROPERTY_CHANGE: {
        const auto* prop = static_cast<const mpv_event_property*>(event->data);
        if (!prop || !prop->data) {
            break;
        }
        if (std::strcmp(prop->name, "time-pos") == 0 && prop->format == MPV_FORMAT_DOUBLE) {
            position = *static_cast<const double*>(prop->data);
        } else if (std::strcmp(prop->name, "duration") == 0 && prop->format == MPV_FORMAT_DOUBLE) {
            duration = *static_cast<const double*>(prop->data);
        } else if (std::strcmp(prop->name, "pause") == 0 && prop->format == MPV_FORMAT_FLAG) {
            playing = *static_cast<const int*>(prop->data) == 0;
        }
        break;
    }
    default:
        break;
    }
}

void MpvSession::update_duration() {
    double value = 0.0;
    if (mpv_get_property(mpv, "duration", MPV_FORMAT_DOUBLE, &value) >= 0) {
        duration = value;
    }
}

void MpvSession::update_pause_state() {
    int paused = 1;
    if (mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &paused) >= 0) {
        playing = paused == 0;
    }
}

void MpvSession::poll_events() {
    if (!mpv) {
        return;
    }

    while (true) {
        const mpv_event* event = mpv_wait_event(mpv, 0);
        if (!event || event->event_id == MPV_EVENT_NONE) {
            break;
        }
        handle_event(event);
    }
}

}  // namespace lf

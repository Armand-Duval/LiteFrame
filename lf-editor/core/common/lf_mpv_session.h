#pragma once

#include <functional>

struct mpv_handle;
struct mpv_event;

namespace lf {

struct MpvSession {
    mpv_handle* mpv       = nullptr;
    bool        has_media = false;
    bool        playing   = false;
    double      position  = 0.0;
    double      duration  = 0.0;

    std::function<void()> on_wakeup;

    MpvSession();
    ~MpvSession();

    MpvSession(const MpvSession&)            = delete;
    MpvSession& operator=(const MpvSession&) = delete;

    void handle_event(const mpv_event* event);
    void update_duration();
    void update_pause_state();
    void poll_events();
};

}  // namespace lf

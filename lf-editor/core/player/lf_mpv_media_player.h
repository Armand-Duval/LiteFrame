#pragma once

#include "player/abstract/lf_media_player.h"

namespace lf {

struct MpvSession;

class MpvMediaPlayer : public MediaPlayer {
public:
    explicit MpvMediaPlayer(MpvSession& session);
    ~MpvMediaPlayer() override = default;

    bool open(const std::string& path) override;
    void close() override;
    bool play() override;
    bool pause() override;
    bool seek(double seconds) override;

    bool   is_playing() const override;
    bool   has_media() const override;
    double position_seconds() const override;
    double duration_seconds() const override;

    void poll() override;

private:
    MpvSession& session_;
};

}  // namespace lf

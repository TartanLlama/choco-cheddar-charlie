#ifndef CHOCO_FPS_HPP
#define CHOCO_FPS_HPP

#include "timer.hpp"

namespace choco {
class fps_capper {
public:
    fps_capper() {
        fps_timer.start();
    }

    ~fps_capper() {
        fps_timer.stop();
    }

    void start_frame() {
        cap_timer.start();
    }

    void end_frame() {
        auto frame_ticks = cap_timer.get_ticks();
        if(frame_ticks < s_ticks_per_frame) {
            SDL_Delay(s_ticks_per_frame - frame_ticks);
        }

        n_frames++;
        last_time_ = cap_timer.get_ticks();
        cap_timer.pause();
    }

    double delta() {
        return last_time_? 1.0/last_time_ : 0;
    }

private:
    static constexpr int s_frame_rate = 60;
    static constexpr int s_ticks_per_frame = 1000 / s_frame_rate;

    tl::u32 n_frames;
    timer fps_timer;
    timer cap_timer;
    tl::u64 last_time_;
};
}

#endif

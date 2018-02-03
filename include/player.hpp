#ifndef CHOCO_PLAYER_HPP
#define CHOCO_PLAYER_HPP

#include "vec2.hpp"

namespace choco {
class player {
public:
    void apply_force (vec2<double> force) {
        acceleration_ += force;
    }

    void tick(double delta) {
        velocity_ += acceleration_ * 25;

        acceleration_ = 0;

        velocity_ *= s_velocity_decay;
        velocity_.clamp(-s_max_velocity, s_max_velocity);

        position_ += velocity_ * (delta/5);
    }

    vec2<int> get_position() { vec2<int> ret { static_cast<int>(position_.x), static_cast<int>(position_.y) }; return ret; }
private:
    static constexpr double s_velocity_decay = 0.9;
    static constexpr double s_max_velocity = 300;

    vec2<double> position_{0};
    vec2<double> velocity_{0};
    vec2<double> acceleration_{0};
};
}

#endif

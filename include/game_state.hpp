#ifndef CHOCO_GAME_STATE_HPP
#define CHOCO_GAME_STATE_HPP

#include "player.hpp"
#include "fps.hpp"
#include "controls.hpp"

namespace choco {
class game_state {
public:
    player& get_player() { return player_; }
    controls& get_controls() { return controls_; }

    void start_frame() {
        fps_capper_.start_frame();
    }

    void end_frame() {
        fps_capper_.end_frame();
    }

    void tick() {
        player_.tick(fps_capper_.delta());

        if (controls_.button_pressed(controls::left)) {
            player_.apply_force({-1, 0});
        }
        if (controls_.button_pressed(controls::right)) {
            player_.apply_force({+1, 0});
        }
        if (controls_.button_pressed(controls::up)) {
            player_.apply_force({0, -1});
        }
        if (controls_.button_pressed(controls::down)) {
            player_.apply_force({0, +1});
        }
    }

private:
    player player_{};
    controls controls_{};
    fps_capper fps_capper_{};
};


}

#endif

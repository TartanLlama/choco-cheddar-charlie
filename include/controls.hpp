#ifndef CHOCO_CONTROLS_HPP
#define CHOCO_CONTROLS_HPP

#include <array>

namespace choco {
struct controls {
    enum button {
        left, right, up, down, n_buttons
    };

    void press_button(button b) {
        keymap[b] = true;
    }

    void release_button(button b) {
        keymap[b] = false;
    }

    bool button_pressed(button b) {
        return keymap[b];
    }

    std::array<bool, n_buttons> keymap;
};
}

#endif

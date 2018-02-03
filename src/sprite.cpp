#include "sprite.hpp"

choco::animation choco::to_animation(std::string_view name) {
    if (name == "idle") {
        return animation::idle;
    }
    if (name == "walk") {
        return animation::walk;
    }

    assert(false && "Animation unhandled");
}

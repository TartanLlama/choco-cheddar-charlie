#ifndef CHOCO_EVENT_HANDLER_HPP
#define CHOCO_EVENT_HANDLER_HPP

namespace choco {
    class game_state;

enum class event_action {
    proceed, quit
};

event_action handle_events (game_state& state);
}

#endif

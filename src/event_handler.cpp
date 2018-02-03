#include "event_handler.hpp"
#include "game_state.hpp"
#include "controls.hpp"
#include "SDL.h"

using namespace choco;

event_action choco::handle_events (game_state& state) {
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        //If user closes the window
        if (e.type == SDL_QUIT){
            return event_action::quit;
        }
        //If user presses any key
        if (e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym){
            case SDLK_LEFT:
                state.get_controls().press_button(controls::left);
                break;
            case SDLK_RIGHT:
                state.get_controls().press_button(controls::right);
                break;
            case SDLK_UP:
                state.get_controls().press_button(controls::up);
                break;
            case SDLK_DOWN:
                state.get_controls().press_button(controls::down);
                break;

            default:
                return event_action::quit;
            }
        }
        //If user releases any key
        if (e.type == SDL_KEYUP){
            switch (e.key.keysym.sym){
            case SDLK_LEFT:
                state.get_controls().release_button(controls::left);
                break;
            case SDLK_RIGHT:
                state.get_controls().release_button(controls::right);
                break;
            case SDLK_UP:
                state.get_controls().release_button(controls::up);
                break;
            case SDLK_DOWN:
                state.get_controls().release_button(controls::down);
                break;

            default:
                return event_action::quit;
            }
        }
        //If user clicks the mouse
        if (e.type == SDL_MOUSEBUTTONDOWN){
            return event_action::quit;
        }
    }


    return event_action::proceed;
}

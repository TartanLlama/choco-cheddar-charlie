#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <string>
#include <string_view>
#include <memory>

#include "numeric_aliases.hpp"
#include "sdl_ptr.hpp"
#include "error.hpp"
#include "sprite.hpp"
#include "controls.hpp"
#include "game_state.hpp"
#include "event_handler.hpp"

void change_animations(choco::sprites& sprites, choco::controls& controls) {
    if (controls.button_pressed(choco::controls::left) || controls.button_pressed(choco::controls::right)) {
        sprites.at("charlie").set_animation(choco::animation::walk);
    }
    else {
        sprites.at("charlie").set_animation(choco::animation::idle);
    }
}

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_sdl_error(std::cout, "SDL_Init");
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
        log_sdl_error(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    window_ptr win {SDL_CreateWindow("Choco Cheddar Charlie", 100, 100, 1024, 768,
                                     SDL_WINDOW_SHOWN)};
    if (win == nullptr) {
        log_sdl_error(std::cout, "SDL_CreateWindow");
        SDL_Quit();
        return 1;
    }

    renderer_ptr ren {SDL_CreateRenderer(
            win.get(), -1, SDL_RENDERER_ACCELERATED)};

    SDL_RenderSetScale(ren.get(), 2,2);
    if (ren == nullptr) {
        log_sdl_error(std::cout, "SDL_CreateRenderer");
        SDL_Quit();
        return 1;
    }

    SDL_Event e{};

    int x = 20;
    int y = 20;

    SDL_Surface *s;

/* Creating the surface. */
    s = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);

/* Filling the surface with red color. */
    SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 0, 0, 0));
    auto back = SDL_CreateTextureFromSurface(ren.get(), s);

    auto sprites = choco::load_sprites(std::experimental::filesystem::path{"../res"});

    for (auto& [name, sprite] : sprites) {
        sprite.load(*ren);
    }


    choco::game_state state{};
    while (true){
        state.start_frame();

        auto action = handle_events(state);
        state.tick();

        if (action == choco::event_action::quit) {
            break;
        }

        change_animations(sprites, state.get_controls());

        SDL_RenderClear(ren.get());
        sprites.at("charlie").render(*ren, state.get_player().get_position(), std::chrono::milliseconds{16});
        SDL_RenderPresent(ren.get());

        state.end_frame();
    }


    SDL_Quit();
    return 0;
}

#include "SDL.h"
#include <iostream>
#include <string>
#include <string_view>
#include <memory>

#include "numeric_aliases.hpp"
#include "vec2.hpp"
#include "timer.hpp"

struct sdl_deleter {
  void operator()(SDL_Surface *ptr) {
    if (ptr)
      SDL_FreeSurface(ptr);
  }
  void operator()(SDL_Texture *ptr) {
    if (ptr)
      SDL_DestroyTexture(ptr);
  }
  void operator()(SDL_Renderer *ptr) {
    if (ptr)
      SDL_DestroyRenderer(ptr);
  }
  void operator()(SDL_Window *ptr) {
    if (ptr)
      SDL_DestroyWindow(ptr);
  }
  void operator()(SDL_RWops *ptr) {
    if (ptr)
      SDL_RWclose(ptr);
  }
};

class player {
public:
    void apply_force (vec2<double> force) {
        acceleration_ += force;
    }

    void tick(double delta) {
        velocity_ += acceleration_ * 50;

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

enum class event_action {
    proceed, quit
        };

event_action handle_events (game_state& state) {
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

void log_sdl_error(std::ostream &os, std::string_view msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
}
// Unique Pointers
using surface_ptr = std::unique_ptr<SDL_Surface, sdl_deleter>;
using texture_ptr = std::unique_ptr<SDL_Texture, sdl_deleter>;
using renderer_ptr = std::unique_ptr<SDL_Renderer, sdl_deleter>;
using window_ptr= std::unique_ptr<SDL_Window, sdl_deleter>;
using rwops_ptr = std::unique_ptr<SDL_RWops, sdl_deleter>;

texture_ptr load_texture(std::string_view file, SDL_Renderer &ren) {
  texture_ptr texture = nullptr;
  surface_ptr loaded_image {SDL_LoadBMP(file.data())};

  // If the loading went ok, convert to texture and return the texture
  if (loaded_image != nullptr) {
      texture.reset(SDL_CreateTextureFromSurface(&ren, loaded_image.get()));
    // Make sure converting went ok too
    if (texture == nullptr) {
      log_sdl_error(std::cout, "CreateTextureFromSurface");
    }
  } else {
    log_sdl_error(std::cout, "LoadBMP");
  }
  return texture;
}

void render_texture(SDL_Texture &tex, SDL_Renderer &ren, vec2<int> pos){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = pos.x;
	dst.y = pos.y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(&tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(&ren, &tex, NULL, &dst);
}

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      log_sdl_error(std::cout, "SDL_Init");
    return 1;
  }

  window_ptr win {SDL_CreateWindow("Choco Cheddar Charlie", 100, 100, 640, 480,
                                               SDL_WINDOW_SHOWN)};
  if (win == nullptr) {
      log_sdl_error(std::cout, "SDL_CreateWindow");
    SDL_Quit();
    return 1;
  }

  renderer_ptr ren {SDL_CreateRenderer(
          win.get(), -1, SDL_RENDERER_ACCELERATED)};
  if (ren == nullptr) {
      log_sdl_error(std::cout, "SDL_CreateRenderer");
    SDL_Quit();
    return 1;
  }

  auto background = load_texture("../res/background.bmp", *ren);
  auto charlie = load_texture("../res/Charlie1.bmp", *ren);
  if (!charlie) {
      abort();
  }

      SDL_RenderClear(ren.get());

      int bW, bH;
      SDL_QueryTexture(background.get(), nullptr, nullptr, &bW, &bH);
      render_texture(*background, *ren, {0, 0});
      render_texture(*background, *ren, {bW, 0});
      render_texture(*background, *ren, {0, bH});
      render_texture(*background, *ren, {bW, bH});
      render_texture(*charlie, *ren, {20, 20});

      SDL_RenderPresent(ren.get());

      SDL_Event e{};

      int x = 20;
      int y = 20;

      SDL_Surface *s;

/* Creating the surface. */
      s = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);

/* Filling the surface with red color. */
SDL_FillRect(s, NULL, SDL_MapRGB(s->format, 0, 0, 0));
auto back = SDL_CreateTextureFromSurface(ren.get(), s);

      game_state state{};
      while (true){
          state.start_frame();

          auto action = handle_events(state);
          state.tick();

          if (action == event_action::quit) {
              break;
          }



          SDL_RenderClear(ren.get());
                    render_texture(*back, *ren, {0,0});
          render_texture(*charlie, *ren, state.get_player().get_position());

          SDL_RenderPresent(ren.get());

          state.end_frame();
      }


  SDL_Quit();
  return 0;
}

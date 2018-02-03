#ifndef CHOCO_SDL_PTR_HPP
#define CHOCO_SDL_PTR_HPP

#include <memory>

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

using surface_ptr = std::unique_ptr<SDL_Surface, sdl_deleter>;
using texture_ptr = std::unique_ptr<SDL_Texture, sdl_deleter>;
using renderer_ptr = std::unique_ptr<SDL_Renderer, sdl_deleter>;
using window_ptr= std::unique_ptr<SDL_Window, sdl_deleter>;
using rwops_ptr = std::unique_ptr<SDL_RWops, sdl_deleter>;

#endif

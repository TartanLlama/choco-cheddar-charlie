#include "SDL.h"
#include <iostream>
#include <string>

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


void log_sdl_error(std::ostream &os, std::string_view msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
}
// Unique Pointers
using suface_ptr = std::unique_ptr<SDL_Surface, sdl_deleter>;
using texture_ptr = std::unique_ptr<SDL_Texture, sdl_deleter>;
using renderer_ptr = std::unique_ptr<SDL_Renderer, sdl_deleter>;
using window_ptr= std::unique_ptr<SDL_Window, sdl_deleter>;
using rwops_ptr = std::unique_ptr<SDL_RWops, sdl_deleter>;

texture_ptr load_texture(std::string_view file, SDL_Renderer &ren) {
  texture_ptr texture = nullptr;
  surface_ptr loaded_image = SDL_LoadBMP(file.c_str());
  
  // If the loading went ok, convert to texture and return the texture
  if (loaded_image != nullptr) {
      texture = SDL_CreateTextureFromSurface(&ren, loaded_image.get());
    // Make sure converting went ok too
    if (texture == nullptr) {
      log_sdl_error(std::cout, "CreateTextureFromSurface");
    }
  } else {
    log_sdl_error(std::cout, "LoadBMP");
  }
  return texture;
}

void render_texture(SDL_Texture &tex, SDL_Renderer &ren, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(&tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(&ren, &tex, NULL, &dst);
}

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      log_sdl_error("SDL_Init");
    return 1;
  }

  window_ptr win = SDL_CreateWindow("Choco Cheddar Charlie", 100, 100, 640, 480,
                              SDL_WINDOW_SHOWN);
  if (win == nullptr) {
      log_sdl_error("SDL_CreateWindow");      
    SDL_Quit();
    return 1;
  }

  renderer_ptr ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr) {
      log_sdl_error("SDL_CreateRenderer");      
    SDL_Quit();
    return 1;
  }

  while (true) {
  }

  SDL_Quit();
  return 0;
}

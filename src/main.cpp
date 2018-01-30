#include "SDL.h"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
    return 1;
  }

  auto win = SDL_CreateWindow("Choco Cheddar Charlie", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  if (win == nullptr){
      std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return 1;
  }

  auto ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ren == nullptr){
      SDL_DestroyWindow(win);
      std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
      SDL_Quit();
      return 1;
  }  

  while(true){}

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}

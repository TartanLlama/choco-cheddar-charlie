#ifndef CHOCO_ERROR_HPP
#define CHOCO_ERROR_HPP

#include <string_view>
#include <iostream>
#include <SDL.h>

inline void log_sdl_error(std::ostream &os, std::string_view msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
}

#endif

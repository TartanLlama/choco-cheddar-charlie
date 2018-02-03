#ifndef CHOCO_TEXTURE_HPP
#define CHOCO_TEXTURE_HPP

#include "sdl_ptr.hpp"

texture_ptr load_texture(std::string_view file, SDL_Renderer &ren) {
    texture_ptr texture {IMG_LoadTexture(&ren, file.data())};
  if (!texture) {
      log_sdl_error(std::cout, "LoadTexture");
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

#endif

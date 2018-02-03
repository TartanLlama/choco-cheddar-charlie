#ifndef CHOCO_SPRITE_HPP
#define CHOCO_SPRITE_HPP

#include <unordered_map>
#include <cassert>
#include <iostream>
#include <vector>
#include <chrono>
#include <string_view>
#include <experimental/filesystem>
#include "numeric_aliases.hpp"
#include "error.hpp"
#include "vec2.hpp"
#include "sdl_ptr.hpp"
#include "SDL_image.h"

namespace choco {
struct frame {
    tl::u32 x, y, w, h;
    std::chrono::milliseconds duration;
};

struct frame_range {
    tl::usize start_frame, end_frame;
};

    enum class animation {
        idle, walk
            };

    animation to_animation(std::string_view name);

class sprite {
public:
    sprite(std::string &&file_name, std::vector<frame>&& frames, std::unordered_map<animation, frame_range>&& frame_ranges) :
        file_name_{std::string{"../res/"} + std::move(file_name)},
        frames_{std::move(frames)}, frame_ranges_{std::move(frame_ranges)}
    {}

    void set_animation(animation a) {
        if (current_animation_ == a) {
            return;
        }

        assert(frame_ranges_.count(a));
        current_animation_ = a;
        time_since_last_frame_ = std::chrono::milliseconds{0};
        current_frame_ = frame_ranges_[a].start_frame;
    }

    void render(SDL_Renderer &ren, vec2<int> pos, std::chrono::milliseconds frame_delta){
        time_since_last_frame_ += frame_delta;

        if (time_since_last_frame_ > frames_[current_frame_].duration) {
            time_since_last_frame_ -= frames_[current_frame_].duration;
            auto& range = frame_ranges_[current_animation_];
            current_frame_++;
            if (current_frame_ > range.end_frame) {
                current_frame_ = range.start_frame;
            }
        }

        auto& curr_frame_data = frames_[current_frame_];

	SDL_Rect dst;
	dst.x = pos.x;
	dst.y = pos.y;
        dst.w = curr_frame_data.w;
        dst.h = curr_frame_data.h;

        SDL_Rect src;
        src.x = curr_frame_data.x;
        src.y = curr_frame_data.y;
        src.w = curr_frame_data.w;
        src.h = curr_frame_data.h;

	SDL_RenderCopy(&ren, texture_.get(), &src, &dst);
    }

    void load(SDL_Renderer& ren) {
        texture_.reset(IMG_LoadTexture(&ren, file_name_.data()));
        if (!texture_) {
            log_sdl_error(std::cout, "LoadTexture");
        }
    }

private:

    std::string file_name_;
    animation current_animation_ = animation::idle;
    std::vector<frame> frames_;
    std::unordered_map<animation, frame_range> frame_ranges_;
    std::chrono::milliseconds time_since_last_frame_ = std::chrono::milliseconds{0};
    texture_ptr texture_;
    tl::usize current_frame_ = 0;
};

using sprites = std::unordered_map<std::string, sprite>;
    sprites load_sprites(std::experimental::filesystem::path resources_path);
}

#endif

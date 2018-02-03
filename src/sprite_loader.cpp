// This is its own file because json.hpp takes ages to compile

#include "json.hpp"
#include <fstream>
#include "sprite.hpp"

namespace {
    choco::sprite process_json_file(std::experimental::filesystem::directory_entry const& file_entry) {
        std::ifstream file {file_entry.path()};
        nlohmann::json json;
        json << file;

        std::vector<choco::frame> frames;
        for (auto& f : json["frames"]) {
            auto dims = f["frame"];
            std::chrono::milliseconds duration { tl::u32 {f["duration"]}};
            frames.push_back(choco::frame{dims["x"], dims["y"], dims["w"], dims["h"], duration});
        }

        std::string file_name = json["meta"]["image"];

        std::unordered_map<choco::animation, choco::frame_range> animations;
        for (auto& a : json["meta"]["frameTags"]) {
            auto animation = choco::to_animation(std::string{a["name"]});
            animations[animation] = choco::frame_range{a["from"], a["to"]};
        }

        return { std::move(file_name), std::move(frames), std:move(animations) };
    }
}

    choco::sprites choco::load_sprites(std::experimental::filesystem::path resources_path) {
    auto files = std::experimental::filesystem::directory_iterator(resources_path / "json");
    choco::sprites sprites{};

    for (auto& file : files) {
        sprites.emplace(file.path().stem().string(), process_json_file(file));
    }

    return sprites;
}

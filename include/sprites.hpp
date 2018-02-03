#ifndef CHOCO_SPRITES_HPP
#define CHOCO_SPRITES_HPP

#include <filesystem>

class sprites {
    static sprites load(std::filesystem::path resources_path) {
        auto files = std::filesystem::directory_iterator(resources_path / "json");

        std::vector<sprite> sprites;

        for (auto& file : files) {

        }
    }
};

#endif

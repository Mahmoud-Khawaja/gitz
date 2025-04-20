#include "repository.h"
#include <fstream>
#include <iostream>
#include <stdexcept>


namespace gitz {
    std::optional<std::filesystem::path> Repository::findRepo() {
        std::filesystem::path current = std::filesystem::current_path();
        while (true) {
            std::filesystem::path gitDir = current / ".gitz";
            if (std::filesystem::exists(gitDir) && std::filesystem::is_directory(gitDir)) return current;
            if (current == current.root_directory())break;
            current = current.parent_path();
        }
        return std::nullopt;
    }

    void Repository::init(const std::filesystem::path& path) {
        // TODO
    }
    // setting absulute path
    Repository::Repository(const std::filesystem::path& root) : root_(std::filesystem::canonical(root)), gitDir_(root_ / ".gitz") {
        // TODO 
    }
}
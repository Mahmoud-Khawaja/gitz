#include "include/index.h"
#include <fstream>
#include <stdexcept>

namespace gitz {

Index::Index(const std::filesystem::path& gitDir) : indexPath_(gitDir / "index") {}

void Index::load() {
    entries_.clear();
    if (std::filesystem::exists(indexPath_)) {
        std::ifstream file(indexPath_);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open index file for reading: " + indexPath_.string());
        }
        std::string line;
        while (std::getline(file, line)) {
            size_t spacePos = line.find(' ');
            if (spacePos != std::string::npos) {
                std::string hash = line.substr(0, spacePos);
                std::string path = line.substr(spacePos + 1);
                entries_[path] = hash;
            }
        }
    }
}

void Index::save() const {
    std::ofstream file(indexPath_);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open index file for writing: " + indexPath_.string());
    }
    for (const auto& [path, hash] : entries_) {
        file << hash << " " << path << "\n";
    }
}

void Index::addFile(const std::filesystem::path& filePath, const std::string& blobHash) {
    std::string pathStr = filePath.string();
    entries_[pathStr] = blobHash;
}

void Index::removeFile(const std::filesystem::path& filePath) {
    std::string pathStr = filePath.string();
    entries_.erase(pathStr);
}

void Index::clear() {
    entries_.clear();
}

bool Index::isFileStaged(const std::filesystem::path& filePath) const {
    std::string pathStr = filePath.string();
    return entries_.find(pathStr) != entries_.end();
}

}  
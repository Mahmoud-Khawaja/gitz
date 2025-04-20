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
        std::filesystem::path gitDir = path / ".gitz";
        if (std::filesystem::exists(gitDir)) throw std::runtime_error("omak btgely kol 5amees " + gitDir.string());
    
        try {
            std::filesystem::create_directory(gitDir);
            std::filesystem::create_directory(gitDir / "objects");
            std::filesystem::create_directory(gitDir / "refs" / "heads");
    
            std::ofstream headFile(gitDir / "HEAD");
            if (!headFile) throw std::runtime_error("Failed to create HEAD file");
            headFile << "ref: refs/heads/main";
            headFile.close();
    
            std::ofstream indexFile(gitDir / "index");
            if (!indexFile) throw std::runtime_error("Failed to create index file");
            indexFile.close();
    
            std::cout << "good job nigga! Initialized empty repository in " << gitDir << "\n";
        } catch (const std::exception& e) {
            std::filesystem::remove_all(gitDir); // Cleanup the dog shit
            throw std::runtime_error("Failed to initialize repository: " + std::string(e.what()));
        }
    }
    // setting absulute path
    Repository::Repository(const std::filesystem::path& root) : root_(std::filesystem::canonical(root)), gitDir_(root_ / ".gitz") {
        if (!std::filesystem::exists(gitDir_) || !std::filesystem::is_directory(gitDir_)) throw std::runtime_error("omak btgely kol talat " + root.string());
    }
}
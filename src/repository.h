#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <filesystem>
#include <optional>
#include <string>

namespace gitz {

class Repository {
private:
    std::filesystem::path root_;  
    std::filesystem::path gitDir_; 

public:
    static std::optional<std::filesystem::path> findRepo();

    static void init(const std::filesystem::path& path = ".");

    explicit Repository(const std::filesystem::path& root);

    std::filesystem::path getRoot() const { return root_; }
    std::filesystem::path getGitDir() const { return gitDir_; }
};

} 

#endif 
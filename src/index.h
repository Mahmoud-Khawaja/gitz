#ifndef INDEX_H
#define INDEX_H

#include <filesystem>
#include <map>
#include <string>

namespace gitz {

class Index {
public:
    explicit Index(const std::filesystem::path& gitDir);
    void load();
    void save() const;
    void addFile(const std::filesystem::path& filePath, const std::string& blobHash);
    void removeFile(const std::filesystem::path& filePath);
    const std::map<std::string, std::string>& getEntries() const { return entries_; }
    void clear();

private:
    std::filesystem::path indexPath_;
    std::map<std::string, std::string> entries_; // path -> hash
};

} 

#endif 
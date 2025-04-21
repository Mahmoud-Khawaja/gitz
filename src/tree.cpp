#include "include/tree.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace gitz {

Tree::Tree(const std::vector<Entry>& entries) : entries_(entries) {
    std::sort(entries_.begin(), entries_.end());
}

std::string Tree::serialize() const {
    std::string content;
    for (const auto& entry : entries_) {
        content += entry.mode + " " + entry.type + " " + entry.hash + "\t" + entry.name + "\n";
    }
    return content;
}

std::string Tree::getType() const {
    return "tree";
}

Tree Tree::fromContent(const std::string& content) {
    std::vector<Entry> entries;
    std::istringstream iss(content);
    std::string line;
    while (std::getline(iss, line)) {
        size_t tabPos = line.find('\t');
        if (tabPos == std::string::npos) {
            throw std::runtime_error("Invalid tree entry format");
        }
        std::string beforeTab = line.substr(0, tabPos);
        std::string name = line.substr(tabPos + 1);
        std::istringstream beforeStream(beforeTab);
        std::string mode, type, hash;
        beforeStream >> mode >> type >> hash;
        if (mode.empty() || type.empty() || hash.empty() || name.empty()) {
            throw std::runtime_error("Malformed tree entry");
        }
        entries.push_back({mode, type, hash, name});
    }
    return Tree(entries);
}

}

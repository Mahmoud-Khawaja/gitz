#ifndef TREE_H
#define TREE_H

#include "object.h"
#include <string>
#include <vector>

namespace gitz {

class Tree : public Object {
public:
    struct Entry {
        std::string mode;
        std::string type;
        std::string hash;
        std::string name;

        bool operator<(const Entry& other) const;
    };

    explicit Tree(const std::vector<Entry>& entries);
    std::string serialize() const override;
    std::string getType() const override;
    static Tree fromContent(const std::string& content);
    const std::vector<Entry>& getEntries() const;

private:
    std::vector<Entry> entries_;
};

} 

#endif 

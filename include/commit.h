#ifndef COMMIT_H
#define COMMIT_H

#include "object.h"
#include <string>
#include <ctime>

namespace gitz {

class Commit : public Object {
public:
    Commit(const std::string& treeHash, const std::string& parentHash, const std::string& author,const std::string& committer, const std::string& message);
    std::string serialize() const override;
    std::string getType() const override;
    static Commit fromContent(const std::string& content);
    std::string getTreeHash() const;
    std::string getParentHash() const;
    std::string getMessage() const;
    std::string getAuthor() const;
    time_t getTimestamp() const;

    friend class CommitBuilder;  //helpers for constructing commits

private:
    std::string treeHash_;
    std::string parentHash_;
    std::string author_;
    std::string committer_;
    std::string message_;
    time_t timestamp_;
};

}

#endif 

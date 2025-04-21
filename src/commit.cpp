#include "include/commit.h"
#include <sstream>
#include <string>
#include <ctime>

namespace gitz {

// initializing a commit with tree hash and optional parent hash and a message.
// aslso setting the meta data for the commits 
Commit::Commit(const std::string& treeHash, const std::string& parentHash, const std::string& author,
    const std::string& committer, const std::string& message)
: treeHash_(treeHash), parentHash_(parentHash), author_(author), committer_(committer),
message_(message), timestamp_(time(nullptr)) {}

// from commit -> string 
// the same as the git structure 
std::string Commit::serialize() const {
    std::ostringstream oss;
    oss << "tree " << treeHash_ << "\n";  
    if (!parentHash_.empty())oss << "parent " << parentHash_ << "\n";

    oss << "author " << author_ << " " << timestamp_ << " +0000\n";
    oss << "committer " << committer_ << " " << timestamp_ << " +0000\n";

    oss << "\n";  
    oss << message_;
    return oss.str();
}

std::string Commit::getType() const {
    return "commit";
}

// from string to commit
Commit Commit::fromContent(const std::string& content) {
    std::istringstream iss(content);
    std::string line;
    std::string treeHash, parentHash, author, committer, message;
    time_t timestamp = 0;

    // headers line by line
    while (std::getline(iss, line) && !line.empty()) {
        if (line.size() >= 5 && line.substr(0, 5) == "tree ") treeHash = line.substr(5);  // tree hash
        else if (line.size() >= 7 && line.substr(0, 7) == "parent ") parentHash = line.substr(7);  // parent hash
        else if (line.size() >= 7 && line.substr(0, 7) == "author ") {
            // Extract author name,email and timestamp
            size_t lastSpace = line.rfind(' ');
            if (lastSpace != std::string::npos) {
                size_t secondLastSpace = line.rfind(' ', lastSpace - 1);
                if (secondLastSpace != std::string::npos) {
                    author = line.substr(7, secondLastSpace - 7);
                    std::string timestampStr = line.substr(secondLastSpace + 1, lastSpace - secondLastSpace - 1);
                    timestamp = std::stoll(timestampStr);  // time from string to int
                }
            }
        } else if (line.size() >= 10 && line.substr(0, 10) == "committer ") {
            // Extract committer name/email (timestamp ignored, reused from author)
            size_t lastSpace = line.rfind(' ');
            if (lastSpace != std::string::npos) {
                size_t secondLastSpace = line.rfind(' ', lastSpace - 1);
                if (secondLastSpace != std::string::npos)committer = line.substr(10, secondLastSpace - 10);
            }
        }
    }

    //getting remaining lines as the commit message
    std::string msgLine;
    while (std::getline(iss, msgLine)) {
        message += msgLine + "\n";
    }

    // remove trailing newline from the message 
    if (!message.empty())message.pop_back();

    // construct the commit 
    Commit commit(treeHash, parentHash, author, committer, message);
    commit.author_ = author;
    commit.committer_ = committer;
    commit.timestamp_ = timestamp;
    return commit;
}


std::string Commit::getTreeHash() const {
    return treeHash_;
}


std::string Commit::getParentHash() const {
    return parentHash_;
}

std::string Commit::getMessage() const {
    return message_;
}
std::string Commit::getAuthor() const{
    return author_;
}
time_t Commit::getTimestamp() const{
    return timestamp_;
}

} 

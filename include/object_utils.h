#ifndef OBJECT_UTILS_H
#define OBJECT_UTILS_H

#include "object.h"
#include <filesystem>
#include <string>
#include <utility>

namespace gitz {

// compute the SHA-1
std::string computeHash(const std::string& data);

// write an object to the repository's object and return the hash
std::string writeObject(const Object& obj, const std::filesystem::path& gitDir);

// read an object meta data from hash 
std::pair<std::string, std::string> readObjectContent(const std::string& hash, const std::filesystem::path& gitDir);

}  

#endif  
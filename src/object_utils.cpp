#include "object_utils.h"
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <stdexcept>

namespace gitz {

std::string computeHash(const std::string& data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    std::stringstream ss;

    // here we are converting the binary to hexa string 
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) ss << std::setw(2) << static_cast<int>(hash[i]);
    return ss.str();
}
// writing a serialized Git object and return hash 
std::string writeObject(const Object& obj, const std::filesystem::path& gitDir) {
    /*
    (git contnent)
    git header -> "<type> <size>\0" 
    take care of the nullchar at the end 
    */
    std::string content = obj.serialize();
    std::string header = obj.getType() + " " + std::to_string(content.size()) + '\0';
    std::string fullContent = header + content;
    std::string hash = computeHash(fullContent);
    //cout << hash << "\n";


    // .git/objects/<first 2 chars>/<rest>
    std::filesystem::path objPath = gitDir/"objects"/hash.substr(0, 2) / hash.substr(2);
    std::filesystem::create_directories(objPath.parent_path());

    // writing the content 
    std::ofstream objFile(objPath, std::ios::binary);
    if (!objFile) throw std::runtime_error("error zany because of object writing " + objPath.string());
    objFile.write(fullContent.data(), fullContent.size());
    objFile.close();
    return hash;
}

std::pair<std::string, std::string> readObjectContent(const std::string& hash, const std::filesystem::path& gitDir) {
    std::filesystem::path objPath = gitDir/"objects" / hash.substr(0, 2) / hash.substr(2);

    if (!std::filesystem::exists(objPath)) {
        throw std::runtime_error("Object not found: " + hash);
    }

    // reading the content 
    std::ifstream objFile(objPath, std::ios::binary);
    if (!objFile) {
        throw std::runtime_error("Failed to read object: " + hash);
    }


    std::string content((std::istreambuf_iterator<char>(objFile)), std::istreambuf_iterator<char>());
    objFile.close();

    // The object format is "<type> <size>\0<data>"
    size_t nullPos = content.find('\0');
    if (nullPos == std::string::npos) throw std::runtime_error("Invalid object format: " + hash);

    // getting the header and data 
    std::string header = content.substr(0, nullPos);
    std::string data = content.substr(nullPos + 1);
    
    // getting the type 
    size_t spacePos = header.find(' ');
    if (spacePos == std::string::npos) {
        throw std::runtime_error("Invalid object header: " + hash);
    }
    std::string type = header.substr(0, spacePos);
    return {type, data};
}

} 
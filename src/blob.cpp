#include "include/blob.h"

namespace gitz {

Blob::Blob(const std::string& content) : content_(content) {}

std::string Blob::serialize() const {
    return content_;
}

std::string Blob::getType() const {
    return "blob";
}

std::string Blob::getContent() const {
    return content_;
}

}  
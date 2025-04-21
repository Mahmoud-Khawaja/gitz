#ifndef BLOB_H
#define BLOB_H

#include "object.h"
#include <string>

namespace gitz {

class Blob : public Object {
public:
    explicit Blob(const std::string& content);
    std::string serialize() const override;
    std::string getType() const override;
    std::string getContent() const;

private:
    std::string content_;
};

} 

#endif 
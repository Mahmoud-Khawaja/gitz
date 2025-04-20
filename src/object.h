#ifndef OBJECT_H
#define OBJECT_H

#include <string>

namespace gitz {

class Object {
public:
    virtual std::string serialize() const = 0;
    virtual std::string getType() const = 0;
    virtual ~Object() = default;
};

}

#endif 
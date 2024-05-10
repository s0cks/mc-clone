#ifndef MCC_OBJECT_H
#define MCC_OBJECT_H

#include <string>

namespace mcc {
  class Object {
  protected:
    Object() = default;
  public:
    virtual ~Object() = default;
    virtual std::string ToString() const = 0;
  };
}

#endif //MCC_OBJECT_H
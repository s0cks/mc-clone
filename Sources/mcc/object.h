#ifndef MCC_OBJECT_H
#define MCC_OBJECT_H

#include <string>
#include "mcc/metadata.h"

namespace mcc {
  class Object {
  protected:
    Metadata meta_;

    Object() = default;
    explicit Object(const Metadata& meta):
      meta_(meta) {  
    }

    void SetMeta(const Metadata& rhs) {
      meta_ = rhs;
    }
  public:
    virtual ~Object() = default;

    const Metadata& GetMeta() const {
      return meta_;
    }

    virtual std::string ToString() const = 0;
  };
}

#endif //MCC_OBJECT_H
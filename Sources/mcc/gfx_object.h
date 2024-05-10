#ifndef MCC_GFX_OBJECT_H
#define MCC_GFX_OBJECT_H

#include "mcc/gfx.h"
#include "mcc/object.h"

namespace mcc::gfx {
  class Object : public mcc::Object {
  protected:
    Object() = default;
  public:
    ~Object() override = default;
  };

  template<typename Id>
  class ObjectTemplate : public Object {
  protected:
    Id id_;

    ObjectTemplate() = default;
    explicit ObjectTemplate(const Id id):
      Object(),
      id_(id) {
    }
  public:
    ~ObjectTemplate() override = default;

    Id GetId() const {
      return (Id) id_;
    }
  };
}

#endif //MCC_GFX_OBJECT_H
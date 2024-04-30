#ifndef MCC_IBO_SCOPE_H
#define MCC_IBO_SCOPE_H

#include "mcc/ibo/ibo_id.h"

namespace mcc::ibo {
  class Ibo;
  class IboScope {
  protected:
    Ibo* ibo_;

    explicit IboScope(Ibo* ibo):
      ibo_(ibo) {
    }
  public:
    virtual ~IboScope() = default;
    
    Ibo* GetIbo() const {
      return ibo_;
    }

    IboId GetIboId() const;
  };
}

#include "mcc/ibo/ibo_scope_bind.h"
#include "mcc/ibo/ibo_scope_draw.h"
#include "mcc/ibo/ibo_scope_update.h"

#endif //MCC_IBO_SCOPE_H
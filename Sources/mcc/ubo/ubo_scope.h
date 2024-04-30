#ifndef MCC_UBO_SCOPE_H
#define MCC_UBO_SCOPE_H

#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  class UboScope {
  protected:
    Ubo* ubo_;

    explicit UboScope(Ubo* ubo):
      ubo_(ubo) {
    }
  public:
    virtual ~UboScope() = default;

    Ubo* GetUbo() const {
      return ubo_;
    }

    UboId GetUboId() const {
      return GetUbo()->GetId();
    }
  };
}

#include "mcc/ubo/ubo_scope_bind.h"
#include "mcc/ubo/ubo_scope_mapped.h"
#include "mcc/ubo/ubo_scope_read.h"
#include "mcc/ubo/ubo_scope_update.h"

#endif //MCC_UBO_SCOPE_H
#ifndef MCC_UBO_SCOPE_H
#error "Please #include <mcc/ubo/ubo_scope.h> instead."
#endif //MCC_UBO_SCOPE_H

#ifndef MCC_UBO_SCOPE_UPDATE_H
#define MCC_UBO_SCOPE_BIND_H

#include "mcc/ubo/ubo_scope.h"

namespace mcc::ubo {
  class UboUpdateScope : public UboBindScope {
  public:
    explicit UboUpdateScope(Ubo* ubo):
      UboBindScope(ubo) {
    }
    ~UboUpdateScope() override = default;

    void Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes);

    inline void Update(const uint8_t* bytes, const uint64_t num_bytes) {
      return Update(0, bytes, num_bytes);
    }
  };
}

#endif //MCC_UBO_SCOPE_UPDATE_H
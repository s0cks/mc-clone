#ifndef MCC_IBO_SCOPE_H
#error "Please #include <mcc/ibo/ibo_scope.h> instead."
#endif //MCC_IBO_SCOPE_H

#ifndef MCC_IBO_SCOPE_UPDATE_H
#define MCC_IBO_SCOPE_UPDATE_H

#include "mcc/ibo/ibo_scope_bind.h"

namespace mcc::ibo {
  class IboUpdateScope : public IboBindScope {
  public:
    explicit IboUpdateScope(Ibo* ibo):
      IboBindScope(ibo) {
    }
    ~IboUpdateScope() override = default;

    void Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes);

    void Update(const uint8_t* bytes, const uint64_t num_bytes) {
      return Update(0, bytes, num_bytes);
    }
  };
}

#endif //MCC_IBO_SCOPE_UPDATE_H
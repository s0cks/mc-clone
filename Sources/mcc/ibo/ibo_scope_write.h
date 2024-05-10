#ifndef MCC_IBO_SCOPE_H
#error "Please #include <mcc/ibo/ibo_scope.h> instead."
#endif //MCC_IBO_SCOPE_H

#ifndef MCC_IBO_SCOPE_WRITE_H
#define MCC_IBO_SCOPE_WRITE_H

#include "mcc/ibo/ibo_scope.h"

namespace mcc::ibo {
  template<typename I>
  class IboWriteScope : public IboBindScope,
                       public MappedIboScope {
  public:
    explicit IboWriteScope(Ibo* ibo):
      IboBindScope(ibo),
      MappedIboScope(gfx::kWriteOnly, ibo) {
    }
    ~IboWriteScope() override = default;
    //TODO: implement
  };
}

#endif //MCC_IBO_SCOPE_WRITE_H
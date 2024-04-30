#ifndef MCC_UBO_ID_H
#define MCC_UBO_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace ubo {
    typedef GLuint UboId;
    static constexpr const auto kInvalidUboId = 0;

    static inline bool
    IsInvalidUboId(const UboId id) {
      return id == kInvalidUboId;
    }

    static inline bool
    IsValidUboId(const UboId id) {
      return id >= 0;
    }

    rx::observable<UboId> GenerateUboIds(const int num);

    static inline rx::observable<UboId>
    GenerateUboId() {
      return GenerateUboIds(1);
    }
  }
  using ubo::UboId;
  using ubo::kInvalidUboId;
  using ubo::IsValidUboId;
  using ubo::IsInvalidUboId;
}

#endif //MCC_UBO_ID_H
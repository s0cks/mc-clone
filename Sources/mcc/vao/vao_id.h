#ifndef MCC_VAO_ID_H
#define MCC_VAO_ID_H

#include "mcc/rx.h"
#include "mcc/gfx.h"

namespace mcc {
  namespace vao {
    typedef GLuint VaoId;
    static constexpr const VaoId kInvalidVaoId = 0;

    static inline bool
    IsValidVaoId(const VaoId id) {
      return id != kInvalidVaoId;
    }

    static inline bool
    IsInvalidVaoId(const VaoId id) {
      return id == kInvalidVaoId;
    }

    void GenerateVaoIds(VaoId* ids, const int num_ids);

    static inline VaoId
    GenerateVaoId() {
      VaoId id;
      GenerateVaoIds(&id, 1);
      return id;
    }

    rx::observable<VaoId> GenerateVaoIdsAsync(const int num_ids);

    static inline rx::observable<VaoId>
    GenerateVaoIdAsync() {
      return GenerateVaoIdsAsync(1);
    }
  }
  using vao::VaoId;
  using vao::kInvalidVaoId;
  using vao::IsValidVaoId;
  using vao::IsInvalidVaoId;
}

#endif //MCC_VAO_ID_H
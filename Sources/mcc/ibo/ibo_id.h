#ifndef MCC_IBO_ID_H
#define MCC_IBO_ID_H

#include "mcc/rx.h"
#include "mcc/buffer_object.h"

namespace mcc {
  namespace ibo {
    typedef BufferObjectId IboId;
    static constexpr const IboId kInvalidIboId = 0;

    static inline bool
    IsInvalidIboId(const IboId id) {
      return id == kInvalidIboId;
    }

    static inline bool
    IsValidIboId(const IboId id) {
      return id != kInvalidIboId;
    }

    void GenerateIboIds(IboId* ids, const int num_ids);

    static inline IboId
    GenerateIboId() {
      IboId id;
      GenerateIboIds(&id, 1);
      return id;
    }

    rx::observable<IboId> GenerateIboIdsAsync(const int num_ids);

    static inline rx::observable<IboId>
    GenerateIboIdAsync() {
      return GenerateIboIdsAsync(1);
    }
  }
  using ibo::IboId;
}

#endif //MCC_IBO_ID_H
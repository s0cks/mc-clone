#ifndef MCC_IBO_ID_H
#define MCC_IBO_ID_H

#include "mcc/rx.h"
#include "mcc/buffer_object.h"

namespace mcc {
  namespace ibo {
    typedef BufferObjectId IboId;
    static constexpr const IboId kDefaultIboId = 0;
    static constexpr const IboId kInvalidIboId = -1;

    static inline bool
    IsValidIboId(const IboId id) {
      return id != kInvalidIboId;
    }

    static inline rx::observable<IboId>
    GenerateIboId(const int num = 1) {
      return rx::observable<>::create<IboId>([num](rx::subscriber<IboId> s) {
        IboId ids[num];
        glGenBuffers(num, ids);
        
        for(auto idx = 0; idx < num; idx++)
          s.on_next(idx);
        s.on_completed();
      });
    }
  }
  using ibo::IboId;
  using ibo::kInvalidIboId;
  using ibo::kDefaultIboId;
  using ibo::IsValidIboId;
}

#endif //MCC_IBO_ID_H
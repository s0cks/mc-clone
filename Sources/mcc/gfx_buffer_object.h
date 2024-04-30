#ifndef MCC_GFX_BUFFER_H
#define MCC_GFX_BUFFER_H

#include "mcc/gfx_buffer_object_id.h"

namespace mcc {
  namespace gfx {
    class BufferObject {
    protected:
      BufferObjectId id_;

      explicit BufferObject(const BufferObjectId id):
        id_(id) {
      }
    public:
      virtual ~BufferObject() = default;
      virtual std::string ToString() const = 0;

      BufferObjectId GetId() const {
        return id_;
      }
    };
  }
}

#endif //MCC_GFX_BUFFER_H
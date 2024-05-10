#ifndef MCC_IBO_SCOPE_H
#define MCC_IBO_SCOPE_H

#include "mcc/ibo/ibo_id.h"
#include "mcc/gfx_mapped_buffer_scope.h"

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

  class MappedIboScope : public gfx::MappedBufferScope {
  protected:
    explicit MappedIboScope(const gfx::Access access, Ibo* ibo);
  public:
    ~MappedIboScope() override = default;
    uint64_t GetSize() const;
    uint64_t GetLength() const;
  };
}

#include "mcc/ibo/ibo_scope_bind.h"
#include "mcc/ibo/ibo_scope_draw.h"
#include "mcc/ibo/ibo_scope_read.h"
#include "mcc/ibo/ibo_scope_write.h"
#include "mcc/ibo/ibo_scope_update.h"

#endif //MCC_IBO_SCOPE_H
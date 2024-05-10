#ifndef MCC_GFX_MAPPED_BUFFER_SCOPE_H
#define MCC_GFX_MAPPED_BUFFER_SCOPE_H

#include "mcc/gfx.h"
#include "mcc/region.h"
#include "mcc/platform.h"
#include "mcc/gfx_access.h"

namespace mcc::gfx {
  class MappedBufferScope : public Region {
  public:
    template<typename T>
    class Iterator {
    protected:
      const MappedBufferScope* scope_;
      uword current_;

      inline const MappedBufferScope* scope() const {
        return scope_;
      }
    public:
      explicit Iterator(const MappedBufferScope* scope):
        scope_(scope),
        current_(scope->GetStartingAddress()) {
      }
      ~Iterator() = default;

      bool HasNext() const {
        return current_ < scope()->GetEndingAddress();
      }

      T* Next() {
        const auto next = (T*)current_;
        current_ += sizeof(T);
        return next;
      }
    };
  private:
    GLenum target_;
    Access access_;

    GLenum GetTarget() const {
      return target_;
    }

    static void UnmapBuffer(const GLenum target);
    static uword MapBuffer(const GLenum target, const Access access);
  protected:
    MappedBufferScope(const GLenum target, const Access access, const uword size):
      Region(),
      target_(target),
      access_(access) {
      const auto address = MapBuffer(GetTarget(), GetAccess());
      if(address == UNALLOCATED) {
        DLOG(ERROR) << "failed to map buffer."; //TODO: better error handling.
        return;
      }
      start_ = address;
      size_ = size;
    }
  public:
    ~MappedBufferScope() override {
      if(IsMapped())
        Release();
    }

    bool IsMapped() const {
      return GetStartingAddress() != UNALLOCATED
          && GetSize() > 0;
    }

    Access GetAccess() const {
      return access_;
    }

    bool IsReadOnly() const {
      return GetAccess() == kReadOnly;
    }

    bool IsWriteOnly() const {
      return GetAccess() == kWriteOnly;
    }

    bool IsReadWrite() const {
      return GetAccess() == kReadWrite;
    }

    void Release() {
      MCC_ASSERT(IsMapped());
      return UnmapBuffer(GetTarget());
    }
  };
}

#endif //MCC_GFX_MAPPED_BUFFER_SCOPE_H
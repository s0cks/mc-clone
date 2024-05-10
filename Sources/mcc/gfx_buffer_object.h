#ifndef MCC_GFX_BUFFER_H
#define MCC_GFX_BUFFER_H


#include "mcc/gfx_usage.h"
#include "mcc/gfx_object.h"

namespace mcc {
  namespace gfx {
    class BufferObject : public Object {
    protected:
      uword elem_size_;
      uword length_;
      Usage usage_;

      BufferObject(const uword elem_size,
                   const uword length,
                   const Usage usage):
        Object(),
        elem_size_(elem_size),
        length_(length),
        usage_(usage) {
      }

      void SetElementSize(const uword value) {
        elem_size_ = value;
      }

      void SetLength(const uword value) {
        length_ = value;
      }
    public:
      ~BufferObject() override = default;
      
      uword GetElementSize() const {
        return elem_size_;
      }

      uword GetLength() const {
        return length_;
      }

      inline uword GetTotalSize() const {
        return GetElementSize() * GetLength();
      }

      Usage GetUsage() const {
        return usage_;
      }

      inline bool IsStreamUsage() const {
        return gfx::IsStreamUsage(GetUsage());
      }

      inline bool IsDynamicUsage() const {
        return gfx::IsDynamicUsage(GetUsage());
      }

      inline bool IsStaticUsage() const {
        return gfx::IsStaticUsage(GetUsage());
      }

#define DEFINE_USAGE_TYPE_CHECK(Name, _)              \
      inline bool Is##Name##Usage() const {           \
        return GetUsage() == gfx::k##Name##Usage;     \
      }
      FOR_EACH_GFX_USAGE(DEFINE_USAGE_TYPE_CHECK)
#undef DEFINE_USAGE_TYPE_CHECK
    };

    template<typename Id>
    class BufferObjectTemplate : public BufferObject {
    protected:
      Id id_;

      explicit BufferObjectTemplate(const Id id,
                                    const uword elem_size,
                                    const uword length,
                                    const Usage usage):
        BufferObject(elem_size, length, usage),
        id_(id) {
      }
    public:
      ~BufferObjectTemplate() override = default;

      Id GetId() const {
        return id_;
      }
    };
  }
}

#endif //MCC_GFX_BUFFER_H
#ifndef MCC_VBO_H
#define MCC_VBO_H

#include "mcc/vbo/vbo_id.h"
#include "mcc/vbo/vbo_usage.h"
#include "mcc/vbo/vbo_events.h"
#include "mcc/vbo/vbo_layout.h"

namespace mcc {
  namespace vbo {
    rx::observable<VboEvent*> OnVboEvent();
    rx::observable<VboEvent*> OnVboEvent(const VboId id);

#define DEFINE_ON_VBO_EVENT(Name)                 \
    static inline rx::observable<Name##Event*>    \
    On##Name##Event() {                           \
      return OnVboEvent()                         \
        .filter(Name##Event::Filter)              \
        .map(Name##Event::Cast);                  \
    }                                             \
    static inline rx::observable<Name##Event*>    \
    On##Name##Event(const VboId id) {             \
      return OnVboEvent()                         \
        .filter(Name##Event::FilterBy(id))        \
        .map(Name##Event::Cast);                  \
    }
    FOR_EACH_VBO_EVENT(DEFINE_ON_VBO_EVENT)
#undef DEFINE_ON_VBO_EVENT

    class Vbo {
      friend class VboScope;
      friend class VboBindScope;
    private:
      static void BindVbo(const VboId id);
      static void DeleteVbos(const VboId* ids, const int num_ids);
      static void PutVboData(const uint8_t* bytes, const uint64_t num_bytes, const Usage usage);
      static void UpdateVboData(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes);

      static inline void
      UpdateVboData(const uint8_t* bytes, const uint64_t num_bytes) {
        return UpdateVboData(0, bytes, num_bytes);
      }
      
      static inline void
      DeleteVbo(const VboId id) {
        return DeleteVbos(&id, 1);
      }
      
      static inline void
      BindDefaultVbo() {
        return BindVbo(kDefaultVboId);
      }
    protected:
      VboId id_;
      uint64_t length_;
      uint64_t vertex_size_;

      explicit Vbo(const VboId id,
                   const uint64_t length,
                   const uint64_t vertex_size):
        id_(id),
        length_(length),
        vertex_size_(vertex_size) {
      }
    public:
      virtual ~Vbo() = default;
      std::string ToString() const;

      VboId GetId() const {
        return id_;
      }

      uint64_t GetLength() const {
        return length_;
      }

      uint64_t GetVertexSize() const {
        return vertex_size_;
      }

      uint64_t GetSize() const {
        return GetLength() * GetVertexSize();
      }

      rx::observable<VboEvent*> OnEvent() const {
        return OnVboEvent(GetId());
      }

#define DEFINE_ON_VBO_EVENT(Name)               \
      inline rx::observable<Name##Event*>       \
      On##Name##Event() const {                 \
        return vbo::On##Name##Event(GetId());   \
      }
      FOR_EACH_VBO_EVENT(DEFINE_ON_VBO_EVENT)
#undef DEFINE_ON_VBO_EVENT
    };
  }
  using vbo::Vbo;
}

#endif //MCC_VBO_H
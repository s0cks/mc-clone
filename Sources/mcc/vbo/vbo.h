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
      friend class VboBuilderBase;
    private:
      static void PublishEvent(VboEvent* event);
      static void BindVbo(const VboId id);
      static void DeleteVbos(const VboId* ids, const int num_ids);
      static void PutVboData(const uint8_t* bytes, const uint64_t num_bytes, const VboUsage usage);
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

      template<class E, typename... Args>
      static inline void
      PublishEvent(const Vbo* vbo, Args... args) {
        E event(vbo, args...);
        return PublishEvent((VboEvent*) &event);
      }
    protected:
      VboId id_;
      uint64_t length_;
      uint64_t vertex_size_;
      VboUsage usage_;

      explicit Vbo(const VboId id,
                   const uint64_t length,
                   const uint64_t vertex_size,
                   const VboUsage usage):
        id_(id),
        length_(length),
        vertex_size_(vertex_size),
        usage_(usage) {
      }

      template<class E, typename... Args>
      inline void Publish(Args... args) {
        return PublishEvent<E>(this, args...);
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
      
      VboUsage GetUsage() const {
        return usage_;
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
    private:
      static Vbo* New(const VboId id, const uint64_t length, const uint64_t vertex_size, const VboUsage usage);
    };
  }
  using vbo::Vbo;
}

#endif //MCC_VBO_H
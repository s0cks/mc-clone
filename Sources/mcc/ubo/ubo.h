#ifndef MCC_UBO_H
#define MCC_UBO_H

#include "mcc/ubo/ubo_id.h"
#include "mcc/ubo/ubo_events.h"
#include "mcc/ubo/ubo_registry.h"

namespace mcc {
  namespace ubo {
    const UboRegistry& GetRegistry();
    rx::observable<UboEvent*> OnUboEvent();

    static inline rx::observable<UboEvent*>
    OnUboEvent(const UboId id) {
      return OnUboEvent()
        .filter([id](UboEvent* event) {
          return event
              && event->GetUboId() == id;
        });
    }

#define DEFINE_ON_UBO_EVENT(Name)                               \
    static inline rx::observable<Name##Event*>                  \
    On##Name##Event() {                                         \
      return OnUboEvent()                                       \
        .filter(Name##Event::Filter)                            \
        .map(Name##Event::Cast);                                \
    }                                                           \
    static inline rx::observable<Name##Event*>                  \
    On##Name##Event(const UboId id) {                           \
      return OnUboEvent()                                       \
        .filter(Name##Event::FilterBy(id))                      \
        .map(Name##Event::Cast);                                \
    }
    FOR_EACH_UBO_EVENT(DEFINE_ON_UBO_EVENT)
#undef DEFINE_ON_UBO_EVENT

    class Ubo {
      friend class UboBindScope;
      friend class UboUpdateScope;
      friend class UboBuilderBase;
      friend class UboBuilderScope;
    public:
      static constexpr const auto kGlTarget = GL_UNIFORM_BUFFER;
    protected:
      UboId id_;
      uint64_t elem_size_;
      uint64_t length_;
      GLenum usage_;

      explicit Ubo(const UboId id,
                   const uint64_t elem_size,
                   const uint64_t len,
                   const GLenum usage);

      static void Publish(UboEvent* event);

      template<class E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish((UboEvent*) &event);
      }

      static void BindUbo(const UboId id);
      static void DeleteUbos(const UboId* ids, const uint64_t num_ids);
      static void InitUbo(const uint8_t* bytes, const uint64_t num_bytes, const GLenum usage = GL_DYNAMIC_DRAW); //TODO: add usage
      static void UpdateUbo(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes);

      static inline void
      UpdateUbo(const uint8_t* bytes, const uint64_t num_bytes) {
        return UpdateUbo(0, bytes, num_bytes);
      }

      static inline void
      DeleteUbo(const UboId id) {
        return DeleteUbos(&id, 1);
      }

      static inline void
      UnbindUbo() {
        return BindUbo(0);
      }
    public:
      virtual ~Ubo();
      virtual std::string ToString() const;

      UboId GetId() const {
        return id_;
      }

      virtual uint64_t GetLength() const {
        return length_;
      }

      virtual uint64_t GetElementSize() const {
        return elem_size_;
      }

      uint64_t GetTotalSize() const {
        return GetLength() * GetElementSize();
      }

      GLenum GetUsage() const {
        return usage_;
      }

      inline rx::observable<UboEvent*> OnEvent() const {
        return OnUboEvent(GetId());
      }

#define DEFINE_ON_UBO_EVENT(Name)                           \
      inline rx::observable<Name##Event*>                   \
      On##Name##Event() const {                             \
        return ubo::On##Name##Event(GetId());               \
      }
      FOR_EACH_UBO_EVENT(DEFINE_ON_UBO_EVENT)
#undef DEFINE_ON_UBO_EVENT
    };
  }
  using ubo::Ubo;
}

#endif //MCC_UBO_H
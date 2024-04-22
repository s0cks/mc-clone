#ifndef MCC_VAO_H
#define MCC_VAO_H

#include "mcc/vao/vao_id.h"
#include "mcc/vao/vao_events.h"

namespace mcc {
  namespace vao {
    class Vao;
    rx::observable<VaoEvent*> OnVaoEvent();

#define DEFINE_ON_VAO_EVENT(Name)                                       \
    static inline rx::observable<Name##Event*>                          \
    On##Name##Event() {                                                 \
      return OnVaoEvent()                                               \
        .filter(Name##Event::Filter)                                    \
        .map(Name##Event::Cast);                                        \
    }
    FOR_EACH_VAO_EVENT(DEFINE_ON_VAO_EVENT)
#undef DEFINE_ON_VAO_EVENT

    class Vao {
    public:
      struct VaoIdComparator {
        bool operator() (Vao* lhs, Vao* rhs) const {
          return lhs->GetId() < rhs->GetId();
        }
      };
    protected:
      VaoId id_;

      explicit Vao(const VaoId id);

      static void Publish(VaoEvent* event);

      template<class E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish((VaoEvent*) &event);
      }
    public:
      ~Vao();

      VaoId GetId() const {
        return id_;
      }

      inline bool IsValid() const {
        return IsValidVaoId(GetId());
      }

      inline bool IsDefault() const {
        return IsDefaultVaoId(GetId());
      }

      inline rx::observable<VaoEvent*>
      OnEvent() const {
        return OnVaoEvent()
          .filter([this](VaoEvent* event) {
            return event
                && event->GetVaoId() == GetId();
          });
      }

#define DEFINE_ON_VAO_EVENT(Name)                         \
      inline rx::observable<Name##Event*>                 \
      On##Name##Event() const {                           \
        return OnVaoEvent()                               \
          .filter(Name##Event::FilterBy(GetId()));        \
      }                                                   \
      FOR_EACH_VAO_EVENT(DEFINE_ON_VAO_EVENT)
#undef DEFINE_ON_VAO_EVENT
    public:
      static Vao* New(const VaoId id);

      static inline Vao*
      New() {
        const auto id = GenerateVaoId(1)
          .as_blocking()
          .first();
        if(!IsValidVaoId(id))
          return nullptr;
        return New(id);
      }
    };

    rx::observable<Vao*> GetRegisteredVaos();

    static inline rx::observable<VaoId>
    GetRegisteredVaoIds() {
      return GetRegisteredVaos()
        .map([](Vao* vao) {
          return vao->GetId();
        });
    }
  }
  using vao::Vao;
}

#endif //MCC_VAO_H
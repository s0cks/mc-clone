#ifndef MCC_INDEX_BUFFER_H
#define MCC_INDEX_BUFFER_H

#include "mcc/buffer_object.h"

#include "mcc/ibo/ibo_id.h"
#include "mcc/ibo/ibo_events.h"

namespace mcc {
  namespace ibo {
    class Ibo;
    rx::observable<IboEvent*> OnIboEvent();
    rx::observable<Ibo*> GetRegisteredIbos();

#define DEFINE_ON_IBO_EVENT(Name)                                 \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event() {                                           \
      return OnIboEvent()                                         \
        .filter(Name##Event::Filter)                              \
        .map(Name##Event::Cast);                                  \
    }                                                             \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event(const IboId id) {                             \
      return OnIboEvent()                                         \
        .filter(Name##Event::FilterBy(id))                        \
        .map(Name##Event::Cast);                                  \
    }
    FOR_EACH_IBO_EVENT(DEFINE_ON_IBO_EVENT)
#undef DEFINE_ON_IBO_EVENT

    class Ibo {
    public:
      struct IdComparator {
        bool operator() (Ibo* lhs, Ibo* rhs) const {
          return lhs->GetId() < rhs->GetId();
        }
      };
    protected:
      IboId id_;
      
      explicit Ibo(const IboId id);
      static void Publish(IboEvent* event);

      template<typename E, typename... Args>
      static void
      Publish(Args... args) {
        E event(args...);
        return Publish((IboEvent*) &event);
      }
    public:
      ~Ibo();

      IboId GetId() const {
        return id_;
      }

      rx::observable<IboEvent*> OnEvent() const {
        return OnIboEvent()
          .filter([this](IboEvent* event) {
            return event
                && event->GetIboId() == GetId();
          });
      }

#define DEFINE_ON_IBO_EVENT(Name)                                             \
      rx::observable<Name##Event*> On##Name##Event() const {                  \
        return ibo::On##Name##Event(GetId());                                 \
      }
      FOR_EACH_IBO_EVENT(DEFINE_ON_IBO_EVENT)
#undef DEFINE_ON_IBO_EVENT

      std::string ToString() const;
    public:
      static Ibo* New(const IboId id);

      static inline Ibo*
      New() {
        return GenerateIboId()
          .map([](const IboId& id) {
            return New(id);
          })
          .as_blocking()
          .first();
      }
    };

    static inline rx::observable<IboId>
    GetRegisteredIboIds() {
      return GetRegisteredIbos()
        .map([](Ibo* ibo) {
          return ibo->GetId();
        });
    }
  }
  using ibo::Ibo;
}

#endif //MCC_INDEX_BUFFER_H
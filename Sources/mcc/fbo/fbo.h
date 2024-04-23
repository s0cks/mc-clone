#ifndef MCC_FRAMEBUFFER_H
#define MCC_FRAMEBUFFER_H

#include "mcc/fbo/fbo_id.h"
#include "mcc/fbo/fbo_events.h"

namespace mcc {
  namespace fbo {
    rx::observable<FboEvent*> OnFboEvent();

#define DEFINE_ON_FBO_EVENT(Name)                                   \
    static inline rx::observable<Name##Event*>                      \
    On##Name##Event() {                                             \
      return OnFboEvent()                                           \
        .filter(Name##Event::Filter)                                \
        .map(Name##Event::Cast);                                    \
    }                                                               \
    static inline rx::observable<Name##Event*>                      \
    On##Name##Event(const FboId id) {                               \
      return OnFboEvent()                                           \
        .filter(Name##Event::FilterBy(id))                          \
        .map(Name##Event::Cast);                                    \
    }
    FOR_EACH_FBO_EVENT(DEFINE_ON_FBO_EVENT)
#undef DEFINE_ON_FBO_EVENT

    class Fbo {
    protected:
      FboId id_;

      virtual void Destroy();
    private:
      explicit Fbo(const FboId id):
        id_(id) {
      }
    public:
      virtual ~Fbo() = default;
      virtual std::string ToString() const;

      FboId GetId() const {
        return id_;
      }

      rx::observable<FboEvent*> OnEvent() const;

    };

    static inline rx::observable<FboEvent*>
    OnFboEvent(const FboId id) {
      return OnFboEvent()
        .filter([id](FboEvent* event) {
          return event
              && event->GetFboId() == id;
        });
    }
  }
  using fbo::Fbo;
}

#endif //MCC_FRAMEBUFFER_H
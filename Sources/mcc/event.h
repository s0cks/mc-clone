#ifndef MCC_EVENT_H
#define MCC_EVENT_H

#include <string>
#include "mcc/rx.h"

namespace mcc {
  class Event {
  protected:
    Event() = default;
  public:
    virtual ~Event() = default;
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const = 0;
  };

#define DEFINE_EVENT_PROTOTYPE_TYPE_CHECK(Name)                           \
  virtual Name##Event* As##Name##Event() { return nullptr; }              \
  bool Is##Name##Event() { return As##Name##Event() != nullptr; }

#define DEFINE_EVENT_PROTOTYPE(Types)                                     \
  Types(DEFINE_EVENT_PROTOTYPE_TYPE_CHECK)

#define DECLARE_EVENT_TYPE(Proto, Name)                                   \
  public:                                                                 \
    std::string ToString() const override;                                \
    const char* GetName() const override { return #Name; }                \
    Name##Event* As##Name##Event() override { return this; }              \
    static inline bool                                                    \
    Filter(Proto* event) {                                                \
      return event                                                        \
          && event->Is##Name##Event();                                    \
    }                                                                     \
    static inline Name##Event*                                            \
    Cast(Proto* event) {                                                  \
      MCC_ASSERT(event);                                                  \
      MCC_ASSERT(event->Is##Name##Event());                               \
      return event->As##Name##Event();                                    \
    }
}

#endif //MCC_EVENT_H
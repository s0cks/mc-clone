#ifndef MCC_SETTINGS_H
#define MCC_SETTINGS_H

#include "mcc/settings/settings_events.h"

namespace mcc {
  namespace settings {
    class Settings {
    private:
      static void Publish(SettingsEvent* event);

      template<class E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish((SettingsEvent*) &event);
      }
    public:
      static inline rx::observable<SettingsEvent*> OnEvent();
#define DEFINE_ON_SETTINGS_EVENT(Name)                                  \
      static inline rx::observable<Name##Event*>                        \
      On##Name##Event() {                                               \
        return OnEvent()                                                \
          .filter(Name##Event::Filter)                                  \
          .map(Name##Event::Cast);                                      \
      }
      FOR_EACH_SETTINGS_EVENT(DEFINE_ON_SETTINGS_EVENT)
#undef DEFINE_ON_SETTINGS_EVENT
    };
  }
}

#endif //MCC_SETTINGS_H
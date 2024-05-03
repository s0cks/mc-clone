#ifndef MCC_SETTING_EVENT_H
#define MCC_SETTING_EVENT_H

#include "mcc/event.h"
#include "mcc/common.h"

namespace mcc::settings {
#define FOR_EACH_SETTING_EVENT(V)           \
  V(SettingChanged)

  class Setting;
  class SettingEvent;
#define FORWARD_DECLARE(Name) class Name##Event;
  FOR_EACH_SETTING_EVENT(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class SettingEvent : public Event {
  protected:
    const Setting* setting_;

    explicit SettingEvent(const Setting* setting):
      Event(),
      setting_(setting) {
    }
  public:
    ~SettingEvent() override = default;

    const Setting* GetSetting() const {
      return setting_;
    }
    DEFINE_EVENT_PROTOTYPE(FOR_EACH_SETTING_EVENT);
  };

#define DECLARE_SETTING_EVENT(Name)                                             \
  DECLARE_EVENT_TYPE(SettingEvent, Name)

  class SettingChangedEvent : public SettingEvent {
  protected:
    uword old_;
    uword new_;
  public:
    explicit SettingChangedEvent(const Setting* setting,
                                 const uword old_value,
                                 const uword new_value):
      SettingEvent(setting),
      old_(old_value),
      new_(new_value) {
    }
    ~SettingChangedEvent() override = default;

    uword GetOldValueAddress() const {
      return old_;
    }

    template<typename T>
    T* GetOld() const {
      return (T*) GetOldValueAddress();
    }

    uword GetNewValueAddress() const {
      return new_;
    }

    template<typename T>
    T* GetNew() const {
      return (T*) GetNewValueAddress();
    }
    DECLARE_SETTING_EVENT(SettingChanged);
  };
}

#endif //MCC_SETTING_EVENT_H
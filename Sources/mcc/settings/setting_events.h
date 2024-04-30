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

#define DEFINE_TYPE_CHECK(Name)                                       \
    virtual Name##Event* As##Name##Event() { return nullptr; }        \
    bool Is##Name##Event() { return As##Name##Event() != nullptr; }
    FOR_EACH_SETTING_EVENT(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_SETTING_EVENT(Name)                                             \
  public:                                                                       \
    std::string ToString() const override;                                      \
    const char* GetName() const override { return #Name; }                      \
    Name##Event* As##Name##Event() override { return this; }                    \
    static inline bool                                                          \
    Filter(SettingEvent* event) {                                               \
      return event                                                              \
          && event->Is##Name##Event();                                          \
    }                                                                           \
    static inline Name##Event*                                                  \
    Cast(SettingEvent* event) {                                                 \
      MCC_ASSERT(event);                                                        \
      MCC_ASSERT(event->Is##Name##Event());                                     \
      return event->As##Name##Event();                                          \
    }

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
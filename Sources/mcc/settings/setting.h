#ifndef MCC_SETTING_H
#define MCC_SETTING_H

#include "mcc/settings/setting_events.h"

namespace mcc::settings {
  class Setting {
  private:
    rx::subject<SettingEvent*> events_;
  protected:
    Setting() = default;

    void Publish(SettingEvent* event) {
      MCC_ASSERT(event);
      const auto& subscriber = events_.get_subscriber();
      subscriber.on_next(event);
    }

    template<class E, typename... Args>
    void Publish(Args... args) {
      E event(args...);
      return Publish((SettingEvent*) &event);
    }
  public:
    virtual ~Setting() {
      const auto& subscriber = events_.get_subscriber();
      subscriber.on_completed();
    }

    virtual const char* GetName() const = 0;
    virtual std::string ToString() const = 0;

    rx::observable<SettingEvent*> OnEvent() const {
      return events_.get_observable();
    }

#define DEFINE_ON_SETTING_EVENT(Name)             \
    inline rx::observable<Name##Event*>           \
    On##Name##Event() const {                     \
      return OnEvent()                            \
        .filter(Name##Event::Filter)              \
        .map(Name##Event::Cast);                  \
    }
    FOR_EACH_SETTING_EVENT(DEFINE_ON_SETTING_EVENT)
#undef DEFINE_ON_SETTING_EVENT
  };

#define DECLARE_SETTING(Name)                                     \
  public:                                                         \
    std::string ToString() const override;                        \
    const char* GetName() const override { return #Name; }

  template<typename T>
  class SettingTemplate : public Setting {
  private:
    T value_;
  protected:
    SettingTemplate() = default;
    explicit SettingTemplate(const T& init_value):
      Setting(),
      value_(init_value) {
    }
  public:
    ~SettingTemplate() override = default;

    const T& GetValue() const {
      return value_;
    }

    void SetValue(const T& value) {
      const auto old = value_;
      value_ = value;
      Publish<SettingChangedEvent>(this, (const uword) &old, (const uword) &value_);
    }
  };
}

#endif //MCC_SETTING_H
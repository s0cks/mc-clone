#include "mcc/settings/settings.h"

namespace mcc::settings {
  static rx::subject<SettingsEvent*> events_;

  void Settings::Publish(SettingsEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  rx::observable<SettingsEvent*> Settings::OnEvent() {
    return events_.get_observable();
  }
}
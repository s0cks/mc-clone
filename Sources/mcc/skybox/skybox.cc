#include "mcc/skybox/skybox.h"
#include <sstream>

namespace mcc::skybox {
  rx::subject<SkyboxEvent*> events_;

  rx::observable<SkyboxEvent*> OnSkyboxEvent() {
    return events_.get_observable();
  }

  void Skybox::Publish(SkyboxEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    return subscriber.on_next(event);
  }

  std::string Skybox::ToString() const {
    std::stringstream ss;
    ss << "Skybox(";
    ss << ")";
    return ss.str();
  }
}
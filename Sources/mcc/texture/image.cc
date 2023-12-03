#include "mcc/texture/image.h"

namespace mcc {
  rx::observable<std::string> GetValidImageExtensions() {
    return rxcpp::observable<>::create<std::string>([](const rx::subscriber<std::string> subscriber) {
      subscriber.on_next(".json");
      subscriber.on_next(".png");
      subscriber.on_next(".jpeg");
      subscriber.on_completed();
    });
  }
}
#include "mcc/texture/texture_event.h"
#include <sstream>

namespace mcc::texture {
  std::string TextureCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "TextureCreatedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string TextureDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "TextureDestroyedEvent(";
    ss << ")";
    return ss.str();
  }
}
#include "mcc/keyboard/keyboard_event.h"
#include <sstream>

namespace mcc::keyboard {
  std::string KeyboardCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "KeyboardCreatedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string KeyboardDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "KeyboardDestroyedEvent(";
    ss << ")";
    return ss.str();
  }

  std::string KeyEvent::ToString() const {
    std::stringstream ss;
    ss << "KeyEvent(";
    ss << "code=" << code();
    ss << ")";
    return ss.str();
  }

  std::string KeyPressedEvent::ToString() const {
    std::stringstream ss;
    ss << "KeyPressedEvent(";
    ss << "code=" << code();
    ss << ")";
    return ss.str();
  }

  std::string KeyReleasedEvent::ToString() const {
    std::stringstream ss;
    ss << "KeyReleasedEvent(";
    ss << "code=" << code();
    ss << ")";
    return ss.str();
  }
}
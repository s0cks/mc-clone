#include "mcc/engine/engine_event.h"
#include <sstream>

namespace mcc::engine {
  std::string PreInitEvent::ToString() const {
    std::stringstream ss;
    ss << "PreInitEvent(";
    ss << ")";
    return ss.str();
  }

  std::string InitEvent::ToString() const {
    std::stringstream ss;
    ss << "InitEvent(";
    ss << ")";
    return ss.str();
  }

  std::string PostInitEvent::ToString() const {
    std::stringstream ss;
    ss << "PostInitEvent(";
    ss << ")";
    return ss.str();
  }

  std::string PreTickEvent::ToString() const {
    std::stringstream ss;
    ss << "PreTickEvent(";
    ss << ")";
    return ss.str();
  }

  std::string TickEvent::ToString() const {
    std::stringstream ss;
    ss << "TickEvent(";
    ss << ")";
    return ss.str();
  }

  std::string PostTickEvent::ToString() const {
    std::stringstream ss;
    ss << "PostTickEvent(";
    ss << ")";
    return ss.str();
  }

  std::string TerminatingEvent::ToString() const {
    std::stringstream ss;
    ss << "TerminatingEvent(";
    ss << ")";
    return ss.str();
  }

  std::string TerminatedEvent::ToString() const {
    std::stringstream ss;
    ss << "TerminatedEvent(";
    ss << ")";
    return ss.str();
  }
}
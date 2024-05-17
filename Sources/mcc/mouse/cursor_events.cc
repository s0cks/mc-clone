#include "mcc/mouse/cursor_events.h"
#include <sstream>
#include "mcc/mouse/cursor.h"

namespace mcc::mouse {
  std::string CursorCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "CursorCreatedEvent(";
    ss << "cursor=" << GetCursor()->ToString();
    ss << ")";
    return ss.str(); 
  }

  std::string CursorDeletedEvent::ToString() const {
    std::stringstream ss;
    ss << "CursorDeletedEvent(";
    ss << "cursor=" << GetCursor()->ToString();
    ss << ")";
    return ss.str(); 
  }

  std::string CursorChangedEvent::ToString() const {
    std::stringstream ss;
    ss << "CursorChangedEvent(";
    ss << "cursor=" << GetCursor()->ToString();
    ss << ")";
    return ss.str(); 
  }
}
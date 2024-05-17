#include "mcc/text/text_events.h"
#include <sstream>
#include "mcc/text/text.h"

namespace mcc::text {
  static rx::subject<TextEvent*> events_;

  rx::observable<TextEvent*> OnTextEvent() {
    return events_.get_observable();
  }

  std::string TextCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "TextCreatedEvent(";
    ss << "text=" << GetText()->ToString();
    ss << ")";
    return ss.str();
  }

  std::string TextDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "TextDestroyedEvent(";
    ss << "text=" << GetText()->ToString();
    ss << ")";
    return ss.str();
  }
}
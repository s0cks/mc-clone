#include "mcc/mouse/mouse_event.h"
#include <sstream>

namespace mcc::mouse {

#define DEFINE_TOSTRING(Name)                               \
  std::string Name##Event::ToString() const {               \
    std::stringstream ss;                                   \
    ss << (*this);                                          \
    return ss.str();                                        \
  }
  
  FOR_EACH_MOUSE_EVENT(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
}
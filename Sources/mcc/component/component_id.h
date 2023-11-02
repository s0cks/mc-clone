#ifndef COMPONENT_ID_H
#define COMPONENT_ID_H

#include <cstdint>

namespace mcc {
  typedef uint64_t ComponentId;

  static constexpr const ComponentId kInvalidComponentId = 0;
}

#endif //COMPONENT_ID_H
#ifndef MCC_ENTITY_ID_H
#define MCC_ENTITY_ID_H

#include <cstdint>

namespace mcc {
  typedef uint64_t EntityId;
  static constexpr const EntityId kInvalidEntityId = 0;
}

#endif //MCC_ENTITY_ID_H
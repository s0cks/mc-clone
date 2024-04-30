#include "mcc/ibo/ibo_scope.h"
#include "mcc/ibo/ibo.h"

namespace mcc::ibo {
  void IboUpdateScope::Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
    Ibo::UpdateBufferData(offset, bytes, num_bytes);
  }
}
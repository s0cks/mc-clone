#include "mcc/ubo/ubo_scope.h"
#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  void UboUpdateScope::Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
    Ubo::UpdateUbo(offset, bytes, num_bytes);
  }
}
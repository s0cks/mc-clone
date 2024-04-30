#include "mcc/vbo/vbo_scope.h"
#include "mcc/vbo/vbo.h"

namespace mcc::vbo {
  void VboUpdateScope::Update(const uint64_t offset, const uint8_t* bytes, const uint64_t num_bytes) {
    Vbo::UpdateVboData(offset, bytes, num_bytes);
  }

  void VboUpdateScope::UpdateVboLength(const uint64_t length) {
    GetVbo()->SetLength(length);
  }
}
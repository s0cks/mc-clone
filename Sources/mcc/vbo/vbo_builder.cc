#include "mcc/vbo/vbo_factory.h"

namespace mcc::vbo {
  template<typename... Attributes>
  Vbo* VboBuilderBase::CreateVbo(const VboId id) {
    MCC_ASSERT(IsValidVboId(id));
    const auto total_size = GetLength() * GetVertexLength();
    Vbo::BindVbo(id);
    Vbo::PutVboData(GetVertexData(), total_size, GetUsage());
    BindAndEnableAll<Attributes...>();
    Vbo::BindDefaultVbo();
    return Vbo::New(id, GetLength(), GetVertexLength(), GetUsage());
  }
}
#include "mcc/vbo/vbo_builder.h"

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

  template<typename... Attributes>
  Vbo* VboBuilderBase::CreateVbo(const VboId id, const int32_t num_vertices) {
    MCC_ASSERT(IsValidVboId(id));
    const auto total_size = num_vertices * GetVertexLength();
    Vbo::BindVbo(id);
    Vbo::PutVboData(NULL, total_size, GetUsage());
    BindAndEnableAll<Attributes...>();
    Vbo::BindDefaultVbo();
    return Vbo::New(id, GetLength(), GetVertexLength(), GetUsage());
  }
}
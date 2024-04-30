#include "mcc/ubo/ubo_builder.h"
#include "mcc/ubo/ubo.h"

namespace mcc::ubo {
  Ubo* UboBuilderBase::InitUbo(const UboId id) {
    Ubo::BindUbo(id);
    Ubo::InitUbo(GetData(), GetTotalSize(), GetUsage());
    Ubo::UnbindUbo();
    return new Ubo(id, GetElementSize(), GetLength(), GetUsage());
  }
}
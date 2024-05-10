#include "mcc/shader/shader_set.h"
#include "mcc/shader/shader.h"

namespace mcc::shader {
  ShaderSet::Item::Item(const Shader* shader):
    id_(shader->GetId()),
    type_(shader->GetType()) {
  }
}
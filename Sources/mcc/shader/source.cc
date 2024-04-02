#include "mcc/shader/source.h"

namespace mcc::shader {
  ShaderCodePtr ShaderSource::GetSource() const {
    return std::make_shared<ShaderCode>(this);
  }
}
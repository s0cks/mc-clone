#include "mcc/shader/shader_spec.h"

namespace mcc::shader {
  std::string JsonShaderSpec::GetShaderName() const {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
  }

  ShaderType JsonShaderSpec::GetShaderType() const {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
  }

  ShaderCode* JsonShaderSpec::GetShaderCode() const {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
  }
}
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_unit.h"
#include "mcc/shader/shader_compiler.h"

namespace mcc::shader {
  const std::set<std::string> FragmentShader::kValidExtensions = {
    "json",
    "glsl",
    "frag",
  };

  std::string FragmentShader::ToString() const {
    std::stringstream ss;
    ss << "FragmentShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  FragmentShader* FragmentShader::FromSource(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  FragmentShader* FragmentShader::FromJson(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  FragmentShader* FragmentShader::FromJson(const std::string& value) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }
}
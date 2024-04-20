#include "mcc/shader/shader_spec.h"

namespace mcc::shader {
  std::string JsonShaderSpec::GetShaderNameProperty() const {
    const auto& name = doc().GetNameProperty();
    MCC_ASSERT(name.IsString());
    return std::string(name.GetString(), name.GetStringLength());
  }

  ShaderType JsonShaderSpec::GetShaderTypeProperty() const {
    const auto& typeProperty = doc().GetTypeProperty();
    const auto type = ParseShaderType(typeProperty);
    MCC_ASSERT(type);
    return *type;
  }

  ShaderCode* JsonShaderSpec::GetShaderCode() const {
    if(doc().HasCodeProperty()) {
      const auto& code = doc().GetCodeProperty();
      MCC_ASSERT(code.IsString());
      return ShaderCode::New(GetShaderType(), code);
    } else if(doc().HasFileProperty()) {
      const auto& file = doc().GetFileProperty();
      MCC_ASSERT(file.IsString());
      std::string abs_path(file.GetString(), file.GetStringLength());
      return ShaderCode::FromFile(GetShaderType(), abs_path);
    }
    return nullptr;
  }
}
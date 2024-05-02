#include "mcc/shader/shader_spec.h"

namespace mcc::shader {
  std::string JsonShaderSpec::GetShaderName() const {
    return doc_->GetName();
  }

  ShaderType JsonShaderSpec::GetShaderType() const {
    const auto spec = doc_->GetSpec();
    MCC_ASSERT(spec);
    const auto type = ParseShaderType(spec->GetTypeProperty());
    MCC_ASSERT(type);
    return type.value();
  }

  ShaderCode* JsonShaderSpec::GetShaderCode() const {
    const auto spec = doc_->GetSpec();
    MCC_ASSERT(spec);
    const auto& source = spec->GetSourceProperty();
    MCC_ASSERT(source.IsString());
    const auto type = GetShaderType();
    const auto src = std::string(source.GetString(), source.GetStringLength());
    if(StartsWith(src, "file://")) {
      return ShaderCode::FromFile(type, src);
    } else if(StartsWith(src, "shader://")) {

    }
    return ShaderCode::New(type, src);
  }
}
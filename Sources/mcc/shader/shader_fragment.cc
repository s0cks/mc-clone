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

  FragmentShader* FragmentShader::FromSource(ShaderCode* source) {
    MCC_ASSERT(source);
    const auto id = ShaderCompiler::Compile(source);
    if(IsInvalidShaderId(id)) {
      LOG(ERROR) << "failed to compile: " << source->ToString();
      return nullptr;
    }
    return New(id);
  }

  FragmentShader* FragmentShader::FromSource(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension("vert"));
    return FromSource(ShaderCode::FragmentShaderCodeFromFile(uri));
  }

  FragmentShader* FragmentShader::FromJson(const json::Value& value) {
    if(value.IsString()) {
      const auto val = std::string(value.GetString(), value.GetStringLength());
      return FromSource(val);
    } else if(value.IsObject()) {
      const auto unit = ShaderUnit::NewVertexShaderUnit("");

      const auto val = value.GetObject();
      if(val.HasMember("source") && val["source"].IsString()) {
        const auto& source = val["source"];
        unit->Append(std::string(source.GetString(), source.GetStringLength()));
      }

      if(val.HasMember("sources") && val["source"].IsArray()) {
        const auto& sources = val["sources"].GetArray();
        for(const auto& source : sources) {
          MCC_ASSERT(source.IsString());
          unit->Append(std::string(source.GetString(), source.GetStringLength()));
        }
      }

      const auto id = ShaderCompiler::Compile(unit);
      if(IsInvalidShaderId(id)) {
        LOG(ERROR) << "failed to compile: " << unit->ToString();
        return nullptr;
      }
      return New(id);
    }
    NOT_IMPLEMENTED(FATAL);//TODO: implement
    return nullptr;
  }
}
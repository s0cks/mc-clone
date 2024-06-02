#include "mcc/shader/shader.h"

#include "mcc/shader/shader_json.h"
#include "mcc/shader/shader_compiler.h"

namespace mcc::shader {
  const std::set<std::string> VertexShader::kValidExtensions = {
    "json",
    "glsl",
    "vert",
  };

  VertexShader* VertexShader::New(ShaderUnit* unit) {
    MCC_ASSERT(unit);
    const auto& meta = unit->GetMeta();
    const auto id = ShaderCompiler::Compile(unit);
    return VertexShader::New(meta, id);
  }

  VertexShader* VertexShader::FromSource(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  VertexShader* VertexShader::FromJson(const uri::Uri& uri) {
    ShaderReaderHandler handler(kVertexShader);
    const auto result = json::ParseJson(uri, handler);
    if(!result) {
      LOG(ERROR) << "failed to parse VertexShader json: " << result;
      return nullptr;
    }
    const auto& meta = handler.GetMeta();
    const auto& sources = handler.GetSources();
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  std::string VertexShader::ToString() const {
    std::stringstream ss;
    ss << "VertexShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}
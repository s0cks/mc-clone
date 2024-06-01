#include "mcc/shader/shader.h"

#include "mcc/shader/shader_json.h"
#include "mcc/shader/shader_compiler.h"

namespace mcc::shader {
  const std::set<std::string> VertexShader::kValidExtensions = {
    "json",
    "glsl",
    "vert",
  };

  VertexShader* VertexShader::FromSource(ShaderCode* source) {
    MCC_ASSERT(source);
    const auto id = ShaderCompiler::Compile(source);
    if(IsInvalidShaderId(id)) {
      LOG(ERROR) << "failed to compile: " << source->ToString();
      return nullptr;
    }
    return New(id);
  }

  VertexShader* VertexShader::FromSource(const uri::Uri& uri) {
    if(uri.HasScheme("file")) {
      return FromSource(ShaderCode::VertexShaderCodeFromFile(uri));
    } else if(uri.HasScheme("shader")) {
      const auto base_path = fmt::format("{0:s}/shaders/", FLAGS_resources);
      auto path = uri.path;
      if(!StartsWith(path, base_path))
        path = fmt::format("{0:s}/{1:s}", base_path, path[0] == '/' ? path.substr(1) : path);
      if(!EndsWith(path, ".vert"))
        path = fmt::format("{0:s}.vert", path);
      const auto new_uri = uri::Uri(fmt::format("file://{0:s}", path));
      return FromSource(ShaderCode::VertexShaderCodeFromFile(new_uri));
    }
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
  }

  std::string VertexShader::ToString() const {
    std::stringstream ss;
    ss << "VertexShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}
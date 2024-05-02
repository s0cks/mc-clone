#include <sstream>
#include <unordered_map>

#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_compiler.h"

namespace mcc::shader {
  static rx::subject<ShaderEvent*> events_;

  rx::observable<ShaderEvent*> OnEvent() {
    return events_.get_observable();
  }

  void Shader::Publish(ShaderEvent* event) {
    events_.get_subscriber().on_next(event);
  }

  Shader::Shader(const ShaderId id):
    id_(id) {
    Publish<ShaderCreatedEvent>(id);
  }

#define DEFINE_NEW_SHADER(Name, Ext, GlValue)                         \
  Name##Shader* Name##Shader::New(const uri::Uri& uri) {              \
    MCC_ASSERT(uri.HasScheme("shader"));                              \
    const auto code = shader::ShaderCode::FromFile(uri);              \
    return code ? New(ShaderCompiler::Compile(code)) : nullptr;       \
  }
  FOR_EACH_SHADER_TYPE(DEFINE_NEW_SHADER)
#undef DEFINE_NEW_SHADER

  std::string VertexShader::ToString() const {
    std::stringstream ss;
    ss << "VertexShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string FragmentShader::ToString() const {
    std::stringstream ss;
    ss << "FragmentShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string GeometryShader::ToString() const {
    std::stringstream ss;
    ss << "GeometryShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string TessEvalShader::ToString() const {
    std::stringstream ss;
    ss << "TessEvalShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string TessControlShader::ToString() const {
    std::stringstream ss;
    ss << "TessControlShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}
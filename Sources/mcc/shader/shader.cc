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
    res::ResourceTemplate<res::kShaderType>(),
    id_(id) {
    Publish<ShaderCreatedEvent>(id);
  }

  int Shader::GetShaderiv(const Property property) const {
    GLint value;
    glGetShaderiv(GetId(), property, &value);
    CHECK_GL(FATAL);
    return static_cast<int>(value);
  }

  void Shader::Destroy() {
    const auto id = GetId();
    glDeleteShader(id);
    CHECK_GL(FATAL);
    Publish<ShaderDestroyedEvent>(id);
  }

  std::string Shader::ToString() const {
    std::stringstream ss;
    ss << "Shader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  ShaderRef Shader::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("shader"));
    const auto code = shader::ShaderCode::FromFile(uri);
    if(!code)
      return ShaderRef();
    return New(ShaderCompiler::Compile(code));
  }
}
#include <sstream>
#include <unordered_map>
#include "mcc/shader/shader.h"

namespace mcc::shader {
  const std::set<std::string>& GetValidFileExtensions() {
    static std::set<std::string> kValidExtensions;
    if(kValidExtensions.empty()) {
      kValidExtensions.insert(std::begin(VertexShader::kValidExtensions), std::end(VertexShader::kValidExtensions));
      kValidExtensions.insert(std::begin(FragmentShader::kValidExtensions), std::end(FragmentShader::kValidExtensions));
      kValidExtensions.insert(std::begin(GeometryShader::kValidExtensions), std::end(GeometryShader::kValidExtensions));
      kValidExtensions.insert(std::begin(TessEvalShader::kValidExtensions), std::end(TessEvalShader::kValidExtensions));
      kValidExtensions.insert(std::begin(TessControlShader::kValidExtensions), std::end(TessControlShader::kValidExtensions));
    }
    return kValidExtensions;
  }

  static rx::subject<ShaderEvent*> events_;
  static ShaderRegistry shaders_(events_);

  rx::observable<ShaderEvent*> OnEvent() {
    return events_.get_observable();
  }

  const ShaderRegistry& GetRegistry() {
    return shaders_;
  }

  void Shader::Publish(ShaderEvent* event) {
    events_.get_subscriber().on_next(event);
  }
}
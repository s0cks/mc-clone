#include "mcc/shader/shader_compiler.h"
#include "mcc/flags.h"

namespace mcc::shader {
  ShaderId ShaderCompiler::Compile(const ShaderCodePtr source) {
    if(source->IsEmpty())
      return kInvalidShaderId;
    auto code = source->data();
    const auto length = source->length();
    const auto id = glCreateShader(source->type());
    if(id == kInvalidShaderId)
      return kInvalidShaderId;
    DLOG(INFO) << "compiling " << (*source) << "....";
    glShaderSource(id, 1, &code, &length);
    CHECK_GL(ERROR);
    glCompileShader(id);
    CHECK_GL(ERROR);
    const auto status = ShaderStatus(GL_COMPILE_STATUS, id, source->type());
    DLOG(INFO) << "status: " << status;
    return !status ? kInvalidShaderId : id;
  }

  rx::observable<ShaderId> ShaderCompiler::Compile(rx::observable<ShaderCodePtr> sources) {
    return sources | rx::operators::map([this](const ShaderCodePtr& source) {
      return Compile(source);
    });
  }
}
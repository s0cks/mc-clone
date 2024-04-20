#include "mcc/shader/shader_compiler.h"

#include "mcc/flags.h"
#include "mcc/uv_utils.h"
#include "mcc/shader/shader_status.h"

namespace mcc::shader {
  ShaderId ShaderCompiler::Compile(ShaderCode* code) {
    if(code->IsEmpty())
      return kInvalidShaderId;
    DLOG(INFO) << "compiling ShaderCode....";
    auto data = code->data();
    const auto length = code->length();

    const auto start_ns = uv_hrtime();

    const auto id = glCreateShader(code->GetShaderType());
    if(id == kInvalidShaderId)
      return kInvalidShaderId;
    glShaderSource(id, 1, &data, &length);
    CHECK_GL(ERROR);
    glCompileShader(id);
    CHECK_GL(ERROR);

    const auto stop_ns = uv_hrtime();
    const auto total_ns = (stop_ns - start_ns);

    const auto status = ShaderCompileStatus(id);
    if(!status) {
      LOG(ERROR) << "failed to compile shader " << id << ": " << status;
      DLOG(ERROR) << "code: " << std::endl << (*code);
      return kInvalidShaderId;
    }
    Shader::Publish<ShaderCompiledEvent>(id, total_ns);
    DLOG(INFO) << "compiled shader " << id << ":";
    DLOG(INFO) << "status: " << status;
    DLOG(INFO) << "duration: " << units::time::nanosecond_t(total_ns) << '.';
    DLOG(INFO) << "code: " << std::endl << (*code);
    return id;
  }
}
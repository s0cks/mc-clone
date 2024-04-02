#include "mcc/shader/compiler.h"
#include "mcc/flags.h"

namespace mcc::shader {
  rx::observable<ShaderId> ShaderSourceCompiler::Compile(const ShaderCodePtr source) {
    return rx::observable<>::create<ShaderId>([&source](rx::subscriber<ShaderId> s) {
      DLOG(INFO) << "compiling " << (*source) << "....";
      const auto id = glCreateShader(source->type());
      auto code = source->data();
      const auto length = source->length();
      glShaderSource(id, 1, &code, &length);
      CHECK_GL(ERROR);
      glCompileShader(id);
      CHECK_GL(ERROR);
      const auto status = ShaderStatus(GL_COMPILE_STATUS, id, source->type());
      if(!status) {
        s.on_error(std::make_exception_ptr(ShaderStatusException(status, "failed to compile shader")));
      } else {
        s.on_next(id);
      }
      s.on_completed();
    });
  }
}
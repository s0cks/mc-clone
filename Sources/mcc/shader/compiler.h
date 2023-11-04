#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include "mcc/shader/shader.h"
#include "mcc/shader/source.h"
#include "mcc/shader/compiler_result.h"

namespace mcc::shader {
  template<const ShaderType Type>
  class Compiler {
    DEFINE_NON_COPYABLE_TYPE(Compiler);
  protected:
    ShaderId id_;
    const Source& source_;

    explicit Compiler(const Source& source):
      id_(glCreateShader(Type)),
      source_(source) {
    }

    ShaderId GetShaderId() const {
      return id_;
    }

    static inline CompilerResult 
    Success() {
      return CompilerResult(true, "success.");
    }

    static inline CompilerResult
    Failed(const std::string& message) {
      return CompilerResult(false, message);
    }

    static inline CompilerResult
    CheckCompilerResult(const ShaderId id, Shader* result) {
      GLint success;
      GLchar message[1024];
      glGetShaderiv(id, GL_COMPILE_STATUS, &success);
      if(!success) {
        glGetShaderInfoLog(id, 1024, NULL, message);
        DLOG(INFO) << "failed to compile shader: " << message;
        (*result) = Shader();
        return Failed(std::string(message, 1024));
      }
      (*result) = Shader(id);
      return Success();
    }
  public:
    virtual ~Compiler() = default;
    
    virtual CompilerResult Compile(Shader* result) {
      DLOG(INFO) << "compiling " << (Type) << " shader....";
      auto code = source_.data();
      const auto length = source_.size();
      DLOG(INFO) << "source (" << length << " bytes):";
      DLOG(INFO) << code;
      glShaderSource(GetShaderId(), 1, &code, &length);
      glCompileShader(GetShaderId());
      return CheckCompilerResult(GetShaderId(), result);
    }
  };

#define DEFINE_SHADER_COMPILER(Name)                              \
  class Name##ShaderCompiler : public Compiler<k##Name##Shader> { \
    DEFINE_NON_COPYABLE_TYPE(Name##ShaderCompiler);               \
  private:                                                        \
    explicit Name##ShaderCompiler(const Source& src):             \
      Compiler(src) {                                             \
    }                                                             \
  public:                                                         \
    ~Name##ShaderCompiler() override = default;                   \
    static inline CompilerResult                                  \
    CompileShader(const Source& src, Shader* result) {            \
      Name##ShaderCompiler compiler(src);                         \
      return compiler.Compile(result);                            \
    }                                                             \
  };

  DEFINE_SHADER_COMPILER(Vertex);
  DEFINE_SHADER_COMPILER(Fragment);
  DEFINE_SHADER_COMPILER(Geometry);
  DEFINE_SHADER_COMPILER(TessEval);
  DEFINE_SHADER_COMPILER(TessControl);
#undef DEFINE_SHADER_COMPILER
}

#endif //MCC_SHADER_COMPILER_H
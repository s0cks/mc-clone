#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include "mcc/shader/shader.h"
#include "mcc/shader/source.h"
#include "mcc/shader/compiler_result.h"

namespace mcc::shader {
  class Compiler {
  protected:
    ShaderId id_;
    const Source& source_;

    explicit Compiler(const ShaderId id, const Source& source):
      id_(id),
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
  public:
    virtual ~Compiler() = default;
    DEFINE_NON_COPYABLE_TYPE(Compiler);
  public:
    virtual CompilerResult Compile(Shader* result) = 0;
  };

  template<const ShaderType Type>
  class ShaderCompiler : public Compiler {
  protected:
    ShaderCompiler(const Source& source):
      Compiler(glCreateShader(Type), source) {
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
    ~ShaderCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(ShaderCompiler);

    CompilerResult Compile(Shader* result) override {
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

  class VertexShaderCompiler : public ShaderCompiler<kVertexShader> {
  protected:
    VertexShaderCompiler(const Source& source):
      ShaderCompiler(source) {
    }
  public:
    ~VertexShaderCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(VertexShaderCompiler);

    static inline CompilerResult
    CompileShader(const Source& source, Shader* result) {
      VertexShaderCompiler compiler(source);
      return compiler.Compile(result);
    }
  };

  class FragmentShaderCompiler : public ShaderCompiler<kFragmentShader> {
  protected:
    FragmentShaderCompiler(const Source& source):
      ShaderCompiler(source) {
    }
  public:
    ~FragmentShaderCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(FragmentShaderCompiler);

    static inline CompilerResult
    CompileShader(const Source& source, Shader* result) {
      FragmentShaderCompiler compiler(source);
      return compiler.Compile(result);
    }
  };
}

#endif //MCC_SHADER_COMPILER_H
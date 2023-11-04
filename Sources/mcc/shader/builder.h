#ifndef MCC_SHADER_LINKER_H
#define MCC_SHADER_LINKER_H

#include "mcc/gfx.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/source.h"
#include "mcc/shader/compiler_result.h"

namespace mcc::shader {
  class ProgramBuilder {
  private:
    static inline CompilerResult
    Failed(const std::string& message) {
      return CompilerResult(false, message);
    }

    static inline CompilerResult
    Success() {
      return CompilerResult(true, "Success.");
    }

    static inline CompilerResult
    CheckLinkResult(const ShaderId id, Shader* result) {
      GLint success;
      GLchar message[1024];
      glGetProgramiv(id, GL_LINK_STATUS, &success);
      if(!success) {
        glGetProgramInfoLog(id, 1024, NULL, message);
        DLOG(INFO) << "failed to compile shader: " << message;
        (*result) = Shader();
        return CompilerResult(false, message);
      }
      (*result) = Shader(id);
      return Success();
    }

    static inline CompilerResult
    Link(const ShaderId id, Shader* result) {
      glLinkProgram(id);
      return CheckLinkResult(id, result);
    }
  private:
    ShaderId id_;
  public:
    ProgramBuilder():
      id_(glCreateProgram()) {
      CHECK_GL(FATAL);
    }
    ~ProgramBuilder() = default;
    DEFINE_NON_COPYABLE_TYPE(ProgramBuilder);

    ShaderId GetShaderId() const {
      return id_;
    }

    CompilerResult Attach(const Shader& shader);

    template<class Compiler>
    CompilerResult Attach(const Source& source) {
      Shader shader;
      auto result = Compiler::CompileShader(source, &shader);
      if(!result) 
        return result;
      return Attach(shader);
    }

    CompilerResult AttachVertexShader(const Source& source);
    CompilerResult AttachFragmentShader(const Source& source);
    CompilerResult AttachGeometryShader(const Source& source);
    CompilerResult AttachTessControlShader(const Source& source);
    CompilerResult AttachTessEvalShader(const Source& source);

    CompilerResult Build(Shader* result) {
      return Link(GetShaderId(), result);
    }
  };
}

#endif//MCC_SHADER_LINKER_H
#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include "mcc/shader/shader.h"
#include "mcc/shader/compiler_result.h"

namespace mcc::shader {
  class GlCompiler {
  protected:
    ShaderId id_;

    explicit GlCompiler(const ShaderId id):
      id_(id) {
    }

    ShaderId GetShaderId() const {
      return id_;
    }

    static inline CompilerResult Success() {
      return CompilerResult(true, "success.");
    }
  public:
    virtual ~GlCompiler() = default;
    DEFINE_NON_COPYABLE_TYPE(GlCompiler);
  public:
    virtual CompilerResult Compile(Shader* result) = 0;
  };

  class ShaderCompiler : public GlCompiler {
  protected:
    Buffer& source_;

    ShaderCompiler(const ShaderType type,
                   Buffer& source):
      GlCompiler(glCreateShader(type)),
      source_(source) {
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
        return CompilerResult(false, std::string(message, 1024));
      }
      (*result) = Shader(id);
      return Success();
    }
  public:
    ~ShaderCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(ShaderCompiler);

    CompilerResult Compile(Shader* result) override {
      auto code = reinterpret_cast<const char*>(source_.data());
      const auto length = static_cast<GLint>(source_.write_pos());
      DLOG(INFO) << "compiling shader (" << length << "bytes): " << code;
      glShaderSource(GetShaderId(), 1, &code, &length);
      glCompileShader(GetShaderId());
      return CheckCompilerResult(GetShaderId(), result);
    }
  };

  class VertexShaderCompiler : public ShaderCompiler {
  protected:
    VertexShaderCompiler(Buffer& source):
      ShaderCompiler(kVertexShader, source) {
    }
  public:
    ~VertexShaderCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(VertexShaderCompiler);

    static inline CompilerResult
    CompileShader(Buffer& source, Shader* result) {
      VertexShaderCompiler compiler(source);
      return compiler.Compile(result);
    }

    static inline CompilerResult
    CompileShaderFromSource(const std::string& source, Shader* result) {
      Buffer data(4096);
      if(!data.Put((const uint8_t*) source.data(), source.length()))
        return CompilerResult(false, "cannot put source in buffer.");
      return CompileShader(data, result);
    }

    static inline CompilerResult
    CompileShaderFromFile(const std::string& filename, Shader* result) {
      auto file = fopen(filename.c_str(), "rb");
      DLOG(INFO) << "compiling vertex shader from: " << filename;
      const auto size = GetFilesize(file);
      Buffer data(size);
      if(!data.ReadFrom(file))
        return CompilerResult(false, "failed to read shader from " + filename);
      return CompileShader(data, result);
    }
  };

  class FragmentShaderCompiler : public ShaderCompiler {
  protected:
    FragmentShaderCompiler(Buffer& source):
      ShaderCompiler(kFragmentShader, source) {
    }
  public:
    ~FragmentShaderCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(FragmentShaderCompiler);

    static inline CompilerResult
    CompileShader(Buffer& source, Shader* result) {
      FragmentShaderCompiler compiler(source);
      return compiler.Compile(result);
    }

    static inline CompilerResult
    CompileShaderFromSource(const std::string& source, Shader* result) {
      Buffer data(4096);
      if(!data.Put((const uint8_t*) source.data(), source.length()))
        return CompilerResult(false, "cannot put source in buffer.");
      return CompileShader(data, result);
    }


    static inline CompilerResult
    CompileShaderFromFile(const std::string& filename, Shader* result) {
      const auto file = fopen(filename.c_str(), "rb");
      DLOG(INFO) << "compiling fragment shader from: " << filename;
      const auto size = GetFilesize(file);
      Buffer source(size);
      if(!source.ReadFrom(file))
        return CompilerResult(false, "failed to read shader from " + filename);
      return CompileShader(source, result);
    }
  };

  class ProgramCompiler : public GlCompiler {
  protected:
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
  public:
    ProgramCompiler():
      GlCompiler(glCreateProgram()) {
    }
    ~ProgramCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(ProgramCompiler);

    CompilerResult Attach(const Shader& shader) {
      glAttachShader(GetShaderId(), shader.GetShaderId());
      return Success();
    }

    CompilerResult AttachVertexShader(Buffer& source) {
      Shader shader;
      auto result = VertexShaderCompiler::CompileShader(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilerResult AttachVertexShaderFromSource(const std::string& source) {
      Shader shader;
      auto result = VertexShaderCompiler::CompileShaderFromSource(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilerResult AttachVertexShaderFromFile(const std::string& filename) {
      Shader shader;
      auto result = VertexShaderCompiler::CompileShaderFromFile(filename, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilerResult AttachFragmentShader(Buffer& source) {
      Shader shader;
      auto result = FragmentShaderCompiler::CompileShader(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilerResult AttachFragmentShaderFromFile(const std::string& filename) {
      Shader shader;
      auto result = FragmentShaderCompiler::CompileShaderFromFile(filename, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilerResult AttachFragmentShaderFromSource(const std::string& source) {
      Shader shader;
      auto result = FragmentShaderCompiler::CompileShaderFromSource(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilerResult Compile(Shader* result) override {
      return Link(GetShaderId(), result);
    }
  };
}

#endif //MCC_SHADER_COMPILER_H
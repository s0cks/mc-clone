#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class GlCompiler {
  public:
    class CompilationResult {
    protected:
      bool success_;
      std::string message_;
    public:
      CompilationResult():
        success_(false),
        message_() {
      }
      CompilationResult(const bool success,
                        const std::string& message):
        success_(success),
        message_(message) {
      }
      CompilationResult(const CompilationResult& rhs):
        success_(rhs.success_),
        message_(rhs.message_) {
      }
      ~CompilationResult() = default;

      bool success() const {
        return success_;
      }

      std::string message() const {
        return message_;
      }

      explicit operator bool() const {
        return success_;
      }

      void operator=(const CompilationResult& rhs) {
        success_ = rhs.success_;
        message_ = rhs.message_;
      }

      friend std::ostream& operator<<(std::ostream& stream, const CompilationResult& rhs)  {
        stream << "CompilationResult(";
        stream << "success=" << rhs.success_ << ", ";
        stream << "message=" << rhs.message_;
        stream << ")";
        return stream;
      }
    };
  protected:
    ShaderId id_;

    explicit GlCompiler(const ShaderId id):
      id_(id) {
    }

    ShaderId GetShaderId() const {
      return id_;
    }

    static inline CompilationResult Success() {
      return CompilationResult(true, "success.");
    }
  public:
    virtual ~GlCompiler() = default;
    DEFINE_NON_COPYABLE_TYPE(GlCompiler);
  public:
    virtual CompilationResult Compile(Shader* result) = 0;
  };

  class ShaderCompiler : public GlCompiler {
  protected:
    Buffer& source_;

    ShaderCompiler(const ShaderType type,
                   Buffer& source):
      GlCompiler(glCreateShader(type)),
      source_(source) {
    }

    static inline CompilationResult
    CheckCompilationResult(const ShaderId id, Shader* result) {
      GLint success;
      GLchar message[1024];
      glGetShaderiv(id, GL_COMPILE_STATUS, &success);
      if(!success) {
        glGetShaderInfoLog(id, 1024, NULL, message);
        DLOG(INFO) << "failed to compile shader: " << message;
        (*result) = Shader();
        return CompilationResult(false, std::string(message, 1024));
      }
      (*result) = Shader(id);
      return Success();
    }
  public:
    ~ShaderCompiler() override = default;
    DEFINE_NON_COPYABLE_TYPE(ShaderCompiler);

    CompilationResult Compile(Shader* result) override {
      auto code = reinterpret_cast<const char*>(source_.data());
      const auto length = static_cast<GLint>(source_.write_pos());
      DLOG(INFO) << "compiling shader (" << length << "bytes): " << code;
      glShaderSource(GetShaderId(), 1, &code, &length);
      glCompileShader(GetShaderId());
      return CheckCompilationResult(GetShaderId(), result);
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

    static inline CompilationResult
    CompileShader(Buffer& source, Shader* result) {
      VertexShaderCompiler compiler(source);
      return compiler.Compile(result);
    }

    static inline CompilationResult
    CompileShaderFromSource(const std::string& source, Shader* result) {
      Buffer data(4096);
      if(!data.Put((const uint8_t*) source.data(), source.length()))
        return CompilationResult(false, "cannot put source in buffer.");
      return CompileShader(data, result);
    }

    static inline CompilationResult
    CompileShaderFromFile(const std::string& filename, Shader* result) {
      auto file = fopen(filename.c_str(), "rb");
      DLOG(INFO) << "compiling vertex shader from: " << filename;
      const auto size = GetFilesize(file);
      Buffer data(size);
      if(!data.ReadFrom(file))
        return CompilationResult(false, "failed to read shader from " + filename);
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

    static inline CompilationResult
    CompileShader(Buffer& source, Shader* result) {
      FragmentShaderCompiler compiler(source);
      return compiler.Compile(result);
    }

    static inline CompilationResult
    CompileShaderFromSource(const std::string& source, Shader* result) {
      Buffer data(4096);
      if(!data.Put((const uint8_t*) source.data(), source.length()))
        return CompilationResult(false, "cannot put source in buffer.");
      return CompileShader(data, result);
    }


    static inline CompilationResult
    CompileShaderFromFile(const std::string& filename, Shader* result) {
      const auto file = fopen(filename.c_str(), "rb");
      DLOG(INFO) << "compiling fragment shader from: " << filename;
      const auto size = GetFilesize(file);
      Buffer source(size);
      if(!source.ReadFrom(file))
        return CompilationResult(false, "failed to read shader from " + filename);
      return CompileShader(source, result);
    }
  };

  class ProgramCompiler : public GlCompiler {
  protected:
    static inline CompilationResult
    CheckLinkResult(const ShaderId id, Shader* result) {
      GLint success;
      GLchar message[1024];
      glGetProgramiv(id, GL_LINK_STATUS, &success);
      if(!success) {
        glGetProgramInfoLog(id, 1024, NULL, message);
        DLOG(INFO) << "failed to compile shader: " << message;
        (*result) = Shader();
        return CompilationResult(false, message);
      }
      (*result) = Shader(id);
      return Success();
    }

    static inline CompilationResult
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

    CompilationResult Attach(const Shader& shader) {
      glAttachShader(GetShaderId(), shader.GetShaderId());
      return Success();
    }

    CompilationResult AttachVertexShader(Buffer& source) {
      Shader shader;
      auto result = VertexShaderCompiler::CompileShader(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilationResult AttachVertexShaderFromSource(const std::string& source) {
      Shader shader;
      auto result = VertexShaderCompiler::CompileShaderFromSource(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilationResult AttachVertexShaderFromFile(const std::string& filename) {
      Shader shader;
      auto result = VertexShaderCompiler::CompileShaderFromFile(filename, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilationResult AttachFragmentShader(Buffer& source) {
      Shader shader;
      auto result = FragmentShaderCompiler::CompileShader(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilationResult AttachFragmentShaderFromFile(const std::string& filename) {
      Shader shader;
      auto result = FragmentShaderCompiler::CompileShaderFromFile(filename, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilationResult AttachFragmentShaderFromSource(const std::string& source) {
      Shader shader;
      auto result = FragmentShaderCompiler::CompileShaderFromSource(source, &shader);
      if(!result)
        return result;
      return Attach(shader);
    }

    CompilationResult Compile(Shader* result) override {
      return Link(GetShaderId(), result);
    }
  };
}

#endif //MCC_SHADER_COMPILER_H
#ifndef MCC_SHADER_H
#define MCC_SHADER_H

#include <glog/logging.h>

#include "mcc/gfx.h"
#include "mcc/buffer.h"
#include "mcc/common.h"

namespace mcc::shader {
  typedef GLuint ShaderId;

  static constexpr const ShaderId kUnknownShaderId = 0;

  enum ShaderType : GLenum {
    kCompositeShader = 1,
    kVertexShader = GL_VERTEX_SHADER,
    kFragmentShader = GL_FRAGMENT_SHADER,
    kGeometryShader = GL_GEOMETRY_SHADER,
    kTessEvalShader = GL_TESS_EVALUATION_SHADER,
    kTessControlShader = GL_TESS_CONTROL_SHADER,
  };

  class Shader {
  protected:
    ShaderId id_;
  public:
    Shader(const ShaderId id = kUnknownShaderId):
      id_(id) {
    }
    Shader(const Shader& rhs):
      id_(rhs.id_) {
    }
    virtual ~Shader() = default;

    bool IsCompiled() const {
      return GetShaderId() != kUnknownShaderId;
    }

    ShaderId GetShaderId() const {
      return id_;
    }

    virtual GLint GetUniformLocation(const std::string& name) const {
      return glGetUniformLocation(GetShaderId(), name.c_str());
    }

    virtual void SetVec4(const std::string& name, const glm::vec4 value) const {
      glUniform4fv(GetUniformLocation(name), 1, &value[0]);
      CHECK_GL(FATAL);
    }

    virtual void SetVec3(const std::string& name, const glm::vec3 value) const {
      glUniform3fv(GetUniformLocation(name), 1, &value[0]);
      CHECK_GL(FATAL);
    }

    virtual void SetMat4(const std::string& name, const glm::mat4 value) const {
      glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
      CHECK_GL(FATAL);
    }

    virtual void SetFloat(const std::string& name, const float& value) const {
      glUniform1f(GetUniformLocation(name), value);
      CHECK_GL(FATAL);
    }

    virtual void SetInt(const std::string& name, const GLint value) const {
      glUniform1i(GetUniformLocation(name), value);
      CHECK_GL(FATAL);
    }

    virtual void ApplyShader() const {
      glUseProgram(id_);
      CHECK_GL(FATAL);
    }

    virtual void DeleteShader() const {
      glDeleteShader(id_);
      CHECK_GL(FATAL);
    }

    Shader& operator=(const Shader& rhs) {
      if(id_ == rhs.id_)
        return *this;
      id_ = rhs.id_;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Shader rhs) {
      stream << "Shader(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };

  Shader CompileShader(const std::string& name);
  
  static inline Shader
  GetColorizedShader() {
    return CompileShader("colorized");
  }

  static inline Shader
  GetTextShader() {
    return CompileShader("text");
  }
}

//TODO: remove
namespace mcc {
  using shader::Shader;
  using shader::CompileShader;
  using shader::GetTextShader;
  using shader::GetColorizedShader;
}

#endif //MCC_SHADER_H
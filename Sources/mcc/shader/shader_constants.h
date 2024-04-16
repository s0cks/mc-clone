#ifndef MCC_SHADER_CONSTANTS_H
#define MCC_SHADER_CONSTANTS_H

#include <memory>
#include <cstdint>
#include <fmt/format.h>

#include "mcc/uri.h"
#include "mcc/gfx.h"

namespace mcc::shader {
  typedef GLuint ShaderId;
  static constexpr const ShaderId kUnknownShaderId = 0; //TODO: remove
  static constexpr const ShaderId kInvalidShaderId = 0;

#define FOR_EACH_SHADER_TYPE(V)                             \
  V(Vertex, vert, GL_VERTEX_SHADER)                         \
  V(Fragment, frag, GL_FRAGMENT_SHADER)                     \
  V(Geometry, geom, GL_GEOMETRY_SHADER)                     \
  V(TessControl, tesc, GL_TESS_EVALUATION_SHADER)           \
  V(TessEval, tese, GL_TESS_CONTROL_SHADER)

  enum ShaderType : GLenum {
#define DEFINE_SHADER_TYPE(Name, Ext, GlValue) k##Name##Shader = (GlValue),
    FOR_EACH_SHADER_TYPE(DEFINE_SHADER_TYPE)
#undef DEFINE_SHADER_TYPE

    kProgramShader,
    kNumberOfShaderTypes,
  };

  static inline std::ostream& operator<<(std::ostream& stream, const ShaderType rhs) {
    switch(rhs) {
#define DEFINE_TOSTRING(Name, Ext, GlValue) \
      case ShaderType::k##Name##Shader: return stream << #Name;
      FOR_EACH_SHADER_TYPE(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
      default: return stream << "Unknown Shader Type";
    }
  }

  static inline const char*
  GetExtensionForType(const ShaderType& type) {
    switch(type) {
#define DEFINE_GET_EXTENSION(Name, Ext, GlValue) \
      case ShaderType::k##Name##Shader: return (#Ext);
      FOR_EACH_SHADER_TYPE(DEFINE_GET_EXTENSION)
#undef DEFINE_GET_EXTENSION
      default: return ".glsl";
    }
  }

  class ShaderStatus {
  public:
    static constexpr const size_t kMessageSize = 1024;
  protected:
    ShaderId id_;
    ShaderType type_;

    GLint status_;
    GLint length_;
    GLchar message_[kMessageSize];
  public:
    ShaderStatus() = default;
    ShaderStatus(const GLenum pname, const ShaderId shaderId, const ShaderType type):
      id_(shaderId),
      type_(type),
      status_(),
      message_() {
      glGetShaderiv(id_, pname, &status_);
      CHECK_GL(ERROR);
      if(!status_) {
        glGetShaderInfoLog(id_, ShaderStatus::kMessageSize, &length_, message_);
        CHECK_GL(ERROR);
        DLOG(ERROR) << "message: " << std::string((const char*) message_, length_);
      }
    }
    ShaderStatus(const ShaderStatus& rhs):
      id_(rhs.id()),
      type_(rhs.type()),
      status_(rhs.status()),
      length_(rhs.length_),
      message_() {
      memcpy(&message_[0], &rhs.message_[0], std::min(kMessageSize, length()));
    }
    ~ShaderStatus() = default;
    
    ShaderId id() const {
      return id_;
    }

    ShaderType type() const {
      return type_;
    }

    GLint status() const {
      return status_;
    }

    size_t length() const {
      return static_cast<size_t>(length_);
    }

    std::string message() const {
      return { message_, length() };
    }

    ShaderStatus& operator=(const ShaderStatus& rhs) {
      id_ = rhs.id();
      type_ = rhs.type();
      status_ = rhs.status();
      length_ = rhs.length_;
      memcpy(&message_[0], &rhs.message_[0], std::min(kMessageSize, rhs.length()));
      return *this;
    }

    explicit operator bool() const {
      return status_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ShaderStatus& rhs) {
      stream << "ShaderStatus(";
      stream << "id=" << rhs.id() << ", ";
      stream << "type=" << rhs.type() << ", ";
      stream << "message=" << rhs.message();
      stream << ")";
      return stream;
    }
  };

  class ProgramStatus {
  public:
    static constexpr const size_t kMessageSize = 1024;
  protected:
    ShaderId id_;
    ShaderType type_;

    GLint status_;
    GLint length_;
    GLchar message_[kMessageSize];
  public:
    ProgramStatus() = default;
    ProgramStatus(const GLenum pname, const ShaderId shaderId, const ShaderType type):
      id_(shaderId),
      type_(type),
      status_(),
      message_() {
      glGetProgramiv(id_, pname, &status_);
      CHECK_GL(ERROR);
      if(!status_) {
        glGetProgramInfoLog(id_, ProgramStatus::kMessageSize, &length_, message_);
        CHECK_GL(ERROR);
      }
    }
    ProgramStatus(const ProgramStatus& rhs):
      id_(rhs.id()),
      type_(rhs.type()),
      status_(rhs.status()),
      length_(rhs.length_),
      message_() {
      memcpy(&message_[0], &rhs.message_[0], std::min(kMessageSize, length()));
    }
    ~ProgramStatus() = default;
    
    ShaderId id() const {
      return id_;
    }

    ShaderType type() const {
      return type_;
    }

    GLint status() const {
      return status_;
    }

    size_t length() const {
      return static_cast<size_t>(length_);
    }

    std::string message() const {
      return { message_, length() };
    }

    ProgramStatus& operator=(const ProgramStatus& rhs) {
      id_ = rhs.id();
      type_ = rhs.type();
      status_ = rhs.status();
      length_ = rhs.length_;
      memcpy(&message_[0], &rhs.message_[0], std::min(kMessageSize, rhs.length()));
      return *this;
    }

    explicit operator bool() const {
      return status_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ProgramStatus& rhs) {
      stream << "ProgramStatus(";
      stream << "id=" << rhs.id() << ", ";
      stream << "type=" << rhs.type() << ", ";
      stream << "message=" << rhs.message();
      stream << ")";
      return stream;
    }
  };

  class ShaderStatusException : public std::exception {
  protected:
    ShaderStatus status_;
    std::string message_;
  public:
    explicit ShaderStatusException(const ShaderStatus& status, const std::string& msg):
      exception(),
      status_(status),
      message_(fmt::format("{0:s}: {1:s}", msg, status.message())) {
    }
    ~ShaderStatusException() override = default;

    std::string message() const {
      return message_;
    }

    const char* what() {
      return message_.data();
    }
  };

  class InvalidShaderUriException : public std::exception {
  protected:
    uri::Uri uri_;
    std::string message_;
  public:
    explicit InvalidShaderUriException(const uri::Uri& uri):
      exception(),
      uri_(uri),
      message_(fmt::format("invalid shader uri: {0:s}", (const std::string&) uri)) {
    }
    ~InvalidShaderUriException() override = default;

    uri::Uri uri() const {
      return uri_;
    }

    std::string message() const {
      return message_;
    }

    const char* what() {
      return message_.data();
    }
  };
}

#endif //MCC_SHADER_CONSTANTS_H
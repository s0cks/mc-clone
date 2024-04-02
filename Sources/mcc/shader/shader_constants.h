#ifndef MCC_SHADER_CONSTANTS_H
#define MCC_SHADER_CONSTANTS_H

#include <cstdint>
#include <fmt/format.h>

#include "mcc/uri.h"
#include "mcc/gfx.h"

namespace mcc::shader {
  typedef GLuint ShaderId;
  static constexpr const ShaderId kUnknownShaderId = 0;

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
    static constexpr const auto kMessageSize = 1024;
  protected:
    ShaderId id_;
    ShaderType type_;
    GLint status_;
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
        glGetShaderInfoLog(id_, ShaderStatus::kMessageSize, NULL, message_);
        CHECK_GL(ERROR);
      }
    }
    ShaderStatus(const ShaderStatus& rhs):
      id_(rhs.id()),
      type_(rhs.type()),
      status_(rhs.status()),
      message_() {
      memcpy(&message_[0], &rhs.message_[0], kMessageSize);
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

    std::string message() const {
      return { message_, kMessageSize };
    }

    ShaderStatus& operator=(const ShaderStatus& rhs) {
      id_ = rhs.id();
      type_ = rhs.type();
      status_ = rhs.status();
      memcpy(&message_[0], &rhs.message_[0], kMessageSize);
      return *this;
    }

    explicit operator bool() const {
      return status_;
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
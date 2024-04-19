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
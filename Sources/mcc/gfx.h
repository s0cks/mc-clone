#ifndef MCC_GFX_H
#define MCC_GFX_H

#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glog/logging.h>

namespace mcc {
  static inline std::ostream&
  operator<<(std::ostream& stream, const glm::vec3 rhs) {
    return stream << "(" << rhs[0] << ", " << rhs[1] << ", " << rhs[2] << ")";
  }

  static inline std::ostream&
  operator<<(std::ostream& stream, const glm::vec2 rhs) {
    return stream << "(" << rhs[0] << ", " << rhs[1] << ")";
  }

#ifdef MCC_DEBUG

#define CHECK_GL(Severity) ({                                     \
  const auto error = glGetError();                                \
  LOG_IF(Severity, error != GL_NO_ERROR) << "glError: " << error; \
})

#else

#endif//MCC_DEBUG

}

#endif //MCC_GFX_H
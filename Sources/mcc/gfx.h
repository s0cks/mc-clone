#ifndef MCC_GFX_H
#define MCC_GFX_H

#include <opengl/gl3.h>
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
}

#endif //MCC_GFX_H
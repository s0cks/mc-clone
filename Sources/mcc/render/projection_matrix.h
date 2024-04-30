#ifndef MCC_PROJECTION_MATRIX_H
#define MCC_PROJECTION_MATRIX_H

#include "mcc/glm.h"

namespace mcc {
  class ProjectionMatrix {
  public:
    static void Init();
    static void Set(const glm::mat4 data);
    static glm::mat4 Get();
  };
}

#endif //MCC_PROJECTION_MATRIX_H
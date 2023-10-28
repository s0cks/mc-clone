#ifndef MCC_MESH_H
#define MCC_MESH_H

#include <vector>
#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/vao.h"
#include "mcc/mesh/index.h"
#include "mcc/mesh/vertex.h"

namespace mcc {
  namespace mesh {
    class Mesh {
      DEFINE_NON_COPYABLE_TYPE(Mesh);
    protected:
      Vao vao_;
      Vbo vbo_;
      uint64_t num_vertices_;

      Mesh(const std::vector<glm::vec3>& vertices);
    public:
      virtual ~Mesh() {
        vbo_.Delete();
        vao_.Delete();
      }

      Vao vao() const {
        return vao_;
      }

      Vbo vbo() const {
        return vbo_;
      }

      virtual void Render();
    public:
      static Mesh* New(const std::vector<glm::vec3>& vertices) {
        return new Mesh(vertices);
      }
    };
  }
}

#endif //MCC_MESH_H
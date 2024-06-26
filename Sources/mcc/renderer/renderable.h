#ifndef MCC_RENDERABLE_H
#define MCC_RENDERABLE_H

#include "mcc/mesh/mesh.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"
#include "mcc/component/component.h"
#include "mcc/component/component_state_table.h"

#include "mcc/material.h"

namespace mcc::renderer {
  struct Renderable {
    ShaderRef shader;
    mesh::Mesh* mesh;
    TextureRef texture;
    MaterialPtr material;

    friend std::ostream& operator<<(std::ostream& stream, const Renderable& rhs) {
      stream << "Renderable(";
      stream << "shader=" << rhs.shader << ", ";
      stream << "texture=" << rhs.texture << ", ";
      stream << "mesh=" << (*rhs.mesh) << ", ";
      stream << "material=" << rhs.material;
      stream << ")";
      return stream;
    }

    friend class Renderer;
    DECLARE_COMPONENT(Renderable);
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void Init();
  };
}

#endif //MCC_RENDERABLE_H
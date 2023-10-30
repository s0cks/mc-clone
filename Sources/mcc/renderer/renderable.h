#ifndef MCC_RENDERABLE_H
#define MCC_RENDERABLE_H

#include "mcc/mesh/mesh.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"
#include "mcc/ecs/component_id.h"

namespace mcc::renderer {
  struct Renderable {
    Shader shader;
    mesh::Mesh* mesh;
    texture::Texture texture;

    friend class Renderer;
  private:
    static void SetComponentId(const ComponentId id);
  public:
    static ComponentId GetComponentId();
  };
}

#endif //MCC_RENDERABLE_H
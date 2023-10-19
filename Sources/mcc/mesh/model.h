#ifndef MCC_MODEL_H
#define MCC_MODEL_H

#include "mcc/scene.h"
#include "mcc/mesh/mesh.h"

namespace mcc {
  class Model : public scene::ModelNode {
  protected:
    Model() = default;
    Model(const Shader& shader,
          Mesh* mesh): 
          scene::ModelNode() {
          SetShaderNode(new scene::ShaderNode(shader));
          SetMeshNode(new scene::MeshNode(mesh));
    }
  public:
    ~Model() override = default;
    DEFINE_NON_COPYABLE_TYPE(Model);
  };
}

#endif //MCC_MODEL_H
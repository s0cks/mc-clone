#ifndef MCC_MODEL_H
#define MCC_MODEL_H

#include "mcc/scene.h"
#include "mcc/mesh/mesh.h"

namespace mcc {
  class Model : public scene::ModelNode {
  protected:
    Shader shader_;
    Mesh* mesh_;

    Model():
      shader_(),
      mesh_(nullptr) {
    }
    Model(const Shader& shader,
          Mesh* mesh):
          shader_(shader),
          mesh_(mesh) {
    }
  public:
    ~Model() override = default;
    DEFINE_NON_COPYABLE_TYPE(Model);

    Shader GetShader() const override {
      return shader_;
    }

    Mesh* GetMesh() const override {
      return mesh_;
    }
  };
}

#endif //MCC_MODEL_H
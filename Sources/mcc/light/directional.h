#ifndef MCC_DIRECTIONAL_LIGHT_H
#define MCC_DIRECTIONAL_LIGHT_H

#include "mcc/gfx.h"
#include "mcc/uniform_buffer.h"
#include "mcc/light/light.h"
#include "mcc/component/component.h"

#include "mcc/camera/perspective_camera.h"

namespace mcc::light {
  class DirectionalLightBufferObject;
  struct DirectionalLight : public Light {
    glm::vec4 direction;

    friend std::ostream& operator<<(std::ostream& stream, const DirectionalLight& rhs) {
      stream << "DirectionalLight(";
      stream << "ambient=" << glm::to_string(rhs.ambient) << ", ";
      stream << "diffuse=" << glm::to_string(rhs.diffuse) << ", "; 
      stream << "specular=" << glm::to_string(rhs.specular) << ", ";
      stream << "direction=" << glm::to_string(rhs.direction);
      stream << ")";
      return stream;
    }
    friend class Renderer;
    DECLARE_COMPONENT(DirectionalLight);
  private:
    static void OnPreInit();
    static void OnPostInit();
  public:
    static void Init();
    static DirectionalLightBufferObject* GetBufferObject();
  };

  class DirectionalLightBufferObject : public UniformBufferObjectTemplate<DirectionalLight> {
  public:
    explicit DirectionalLightBufferObject(const BufferObjectId id):
      UniformBufferObjectTemplate(id) {
    }
    DirectionalLightBufferObject():
      UniformBufferObjectTemplate((const uint64_t) 1) {
      glBindBufferBase(GL_UNIFORM_BUFFER, 1, id_);
      CHECK_GL(FATAL);
    }
    ~DirectionalLightBufferObject() override = default;

    void Update(const DirectionalLight* data);
  };
  DEFINE_RESOURCE_SCOPE(DirectionalLightBufferObject);
}

#endif //MCC_DIRECTIONAL_LIGHT_H
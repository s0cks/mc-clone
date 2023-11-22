#ifndef MCC_POINT_LIGHT_H
#define MCC_POINT_LIGHT_H

#include "mcc/gfx.h"
#include "mcc/uniform_buffer.h"
#include "mcc/light/light.h"
#include "mcc/component/component.h"

namespace mcc::light {
  class PointLightUniformBufferObject;
  struct PointLight : public Light {
    glm::vec4 position;
    float constant;
    float linear;
    float quadratic;
    float pad;

    friend std::ostream& operator<<(std::ostream& stream, const PointLight& rhs) {
      stream << "PointLight(";
      stream << "position=" << glm::to_string(rhs.position) << ", ";
      stream << "ambient=" << glm::to_string(rhs.ambient) << ", ";
      stream << "diffuse=" << glm::to_string(rhs.diffuse) << ", "; 
      stream << "specular=" << glm::to_string(rhs.specular) << ", ";
      stream << "constant=" << rhs.constant << ", ";
      stream << "linear=" << rhs.linear << ", ";
      stream << "quadratic=" << rhs.quadratic;
      stream << ")";
      return stream;
    }

    friend class Renderer;
    DECLARE_COMPONENT(PointLight);
  private:
    static void OnPreInit();
    static void OnPostInit();
  public:
    static void Init();
    static PointLightUniformBufferObject* GetUniformBufferObject();
  };

  class PointLightUniformBufferObject : public UniformBufferObjectTemplate<PointLight> {
  public:
    explicit PointLightUniformBufferObject(const BufferObjectId id):
      UniformBufferObjectTemplate(id) {
    }
    PointLightUniformBufferObject():
      UniformBufferObjectTemplate((const uint64_t) 1) {
      glBindBufferBase(GL_UNIFORM_BUFFER, 2, id_);
      CHECK_GL(FATAL);
    }
    ~PointLightUniformBufferObject() override = default;

    void Update(const PointLight* data);
  };
  DEFINE_RESOURCE_SCOPE(PointLightUniformBufferObject);
}


#endif //MCC_POINT_LIGHT_H
#include "mcc/light/point.h"
#include "mcc/engine/engine.h"
#include "mcc/component/component_state_table.h"

#include "mcc/thread_local.h"

namespace mcc::light {
  DEFINE_COMPONENT(PointLight);

  static ThreadLocal<PointLightUniformBufferObject> ubo_;

  void PointLight::OnPreInit() {
    RegisterComponent();
  }

  void PointLight::OnPostInit() {
    ubo_.Set(new PointLightUniformBufferObject());
  }

  void PointLight::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed().subscribe(&OnEntityDestroyed);
  }

  PointLightUniformBufferObject* PointLight::GetUniformBufferObject() {
    return ubo_.Get();
  }

  void PointLightUniformBufferObject::Update(const PointLight* value) {
    PointLightUniformBufferObjectScope scope(*this);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PointLight), (const GLvoid*) value);
    CHECK_GL(FATAL);
  }
}
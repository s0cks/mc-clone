#include "mcc/light/directional.h"
#include "mcc/engine/engine.h"
#include "mcc/component/component_state_table.h"

#include "mcc/thread_local.h"

namespace mcc::light {
  DEFINE_COMPONENT(DirectionalLight);

  static ThreadLocal<DirectionalLightBufferObject> ubo_;

  void DirectionalLight::OnPreInit() {
    RegisterComponent();
  }

  void DirectionalLight::OnPostInit() {
    ubo_.Set(new DirectionalLightBufferObject());
  }

  void DirectionalLight::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed().subscribe(&OnEntityDestroyed);
  }

  DirectionalLightBufferObject* DirectionalLight::GetBufferObject() {
    return ubo_.Get();
  }

  void DirectionalLightBufferObject::Update(const DirectionalLight* value) {
    DirectionalLightBufferObjectScope scope(*this);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLight), (const GLvoid*) value);
    CHECK_GL(FATAL);
  }
}
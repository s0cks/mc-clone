#include "mcc/lighting/ambient_light.h"
#include "mcc/engine/engine.h"

namespace mcc {
  DEFINE_COMPONENT(AmbientLight);

  void AmbientLight::OnPreInit() {
    RegisterComponent();
  }

  void AmbientLight::OnInit() {

  }

  void AmbientLight::OnPostInit() {

  }

  void AmbientLight::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed()
      .subscribe(&OnEntityDestroyed);
  }
}
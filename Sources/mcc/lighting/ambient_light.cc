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
    //TODO: Engine::OnPreInit(&OnPreInit);
    //TODO: Engine::OnInit(&OnInit);
    //TODO: Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed()
      .subscribe(&OnEntityDestroyed);
  }
}
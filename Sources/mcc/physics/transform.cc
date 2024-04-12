#include "mcc/physics/transform.h"
#include "mcc/engine/engine.h"

namespace mcc::physics {
  DEFINE_COMPONENT(Transform);

  void Transform::OnPreInit() {
    RegisterComponent();
  }

  void Transform::OnInit() {

  }

  void Transform::OnPostInit() {

  }

  void Transform::Init() {
    //TODO: Engine::OnPreInit(&OnPreInit);
    //TODO: Engine::OnInit(&OnInit);
    //TODO: Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed()
      .subscribe(&OnEntityDestroyed);
  }
}
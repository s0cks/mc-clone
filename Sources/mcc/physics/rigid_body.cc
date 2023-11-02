#include "mcc/physics/rigid_body.h"
#include "mcc/engine/engine.h"

namespace mcc::physics {
  DEFINE_COMPONENT(RigidBody);

  void RigidBody::OnPreInit() {
    RegisterComponent();
  }
  
  void RigidBody::OnInit() {

  }

  void RigidBody::OnPostInit() {

  }

  void RigidBody::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed()
      .subscribe(&OnEntityDestroyed);
  }
}
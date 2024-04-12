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
    //TODO: Engine::OnPreInit(&OnPreInit);
    //TODO: Engine::OnInit(&OnInit);
    //TODO: Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed()
      .subscribe(&OnEntityDestroyed);
  }
}
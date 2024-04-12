#include "mcc/physics/force.h"
#include "mcc/engine/engine.h"

namespace mcc::physics {
  DEFINE_COMPONENT(Force);

  void Force::OnPreInit() {
    RegisterComponent();
  }

  void Force::OnInit() {

  }

  void Force::OnPostInit() {

  }

  void Force::Init() {
    //TODO: Engine::OnPreInit(&OnPreInit);
    //TODO: Engine::OnInit(&OnInit);
    //TODO: Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed()
      .subscribe(&OnEntityDestroyed);
  }
}
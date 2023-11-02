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
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed()
      .subscribe(&OnEntityDestroyed);
  }
}
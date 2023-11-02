#include "mcc/renderer/renderable.h"
#include "mcc/engine/engine.h"

namespace mcc::renderer {
  DEFINE_COMPONENT(Renderable);

  void Renderable::OnPreInit() {
    RegisterComponent();
  }

  void Renderable::OnInit() {

  }

  void Renderable::OnPostInit() {

  }

  void Renderable::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
    Entity::OnDestroyed().subscribe(&OnEntityDestroyed);
  }
}
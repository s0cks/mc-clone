#include "mcc/renderer/renderable.h"

namespace mcc::renderer {
  DEFINE_COMPONENT(Renderable);

  void Renderable::OnPreInit(engine::PreInitEvent* e) {
    RegisterComponent();
  }

  void Renderable::OnInit(engine::InitEvent* e) {

  }

  void Renderable::OnPostInit(engine::PostInitEvent* e) {

  }

  void Renderable::Init() {
    const auto engine = engine::Engine::GetEngine();
    engine->OnPreInitEvent().subscribe(&OnPreInit);
    engine->OnInitEvent().subscribe(&OnInit);
    engine->OnPostInitEvent().subscribe(&OnPostInit);
    Entity::OnDestroyed().subscribe(&OnEntityDestroyed);
  }
}
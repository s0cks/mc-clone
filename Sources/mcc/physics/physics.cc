#include "mcc/physics/physics.h"
#include "mcc/engine/engine.h"

#include "mcc/physics/force.h"
#include "mcc/physics/transform.h"
#include "mcc/physics/rigid_body.h"

#include "mcc/ecs/system.h"
#include "mcc/ecs/component.h"
#include "mcc/ecs/coordinator.h"

namespace mcc::physics {
  static constexpr const auto kGravityForce = glm::vec3(0.0f, -9.81f, 0.0f);
  static constexpr const auto kNoForce = glm::vec3(0.0f);

  static glm::vec3 gravity_ = kNoForce;

  void PhysicsSimulator::SetGravity(const glm::vec3& value) {
    gravity_ = value;
  }

  glm::vec3 PhysicsSimulator::GetGravity() {
    return gravity_;
  }

  void PhysicsSimulator::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  void PhysicsSimulator::OnPreInit() {
    Components::Register<Transform>();
    Components::Register<RigidBody>();
    Components::Register<Force>();
  }
  
  void PhysicsSimulator::OnInit() {

  }
  
  void PhysicsSimulator::OnPostInit() {
    Systems::Register<PhysicsSimulator>();
    Signature sig;
    sig.set(Components::GetComponentIdForType<RigidBody>());
    sig.set(Components::GetComponentIdForType<Transform>());
    Systems::SetSignature<PhysicsSimulator>(sig);
    DLOG(INFO) << "sig: " << sig;

    Engine::OnTick(&OnTick);
  }

  void PhysicsSimulator::OnTick(const Tick& tick) {
    DLOG(INFO) << "tick.";
    Systems::ForEachEntityInSystem<PhysicsSimulator>([&tick](const Entity& e) {
      auto& rigid_body = Components::GetComponent<RigidBody>(e);
      auto& transform = Components::GetComponent<Transform>(e);
      DLOG(INFO) << "performing physics for: " << e;
      DLOG(INFO) << "before:";
      DLOG(INFO) << " - rigid body: " << rigid_body;
      DLOG(INFO) << " - transform: " << transform;

      auto force = kNoForce;
      force += rigid_body.mass * gravity_;
      rigid_body.velocity += force / rigid_body.mass * static_cast<float>(tick.dts);
      transform.position += rigid_body.velocity * static_cast<float>(tick.dts);

      DLOG(INFO) << "after:";
      DLOG(INFO) << " - rigid body: " << rigid_body;
      DLOG(INFO) << " - transform: " << transform;
    });
  }
}
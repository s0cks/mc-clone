#include "mcc/physics/physics.h"
#include "mcc/engine/engine.h"

#include "mcc/physics/force.h"
#include "mcc/physics/transform.h"
#include "mcc/physics/rigid_body.h"

namespace mcc::physics {
  static constexpr const auto kGravityForce = glm::vec3(0.0f, -9.81f, 0.0f);
  static constexpr const auto kNoForce = glm::vec3(0.0f);

  static Signature signature_;
  static EntitySet tracked_;

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
    
  }
  
  void PhysicsSimulator::OnInit() {
    Entity::OnDestroyed()
      .subscribe([](EntityDestroyedEvent* e) {
        tracked_.erase(e->id);
      });
    Entity::OnSignatureChanged()
      .subscribe([](EntitySignatureChangedEvent* e) {
        const auto& esig = e->signature;
        const auto& eid = e->id;
        if((esig & signature_) == signature_) {
          tracked_.insert(eid);
        } else {
          tracked_.erase(eid);
        }
      });
  }
  
  void PhysicsSimulator::OnPostInit() {
    signature_.set(RigidBody::GetComponentId());
    signature_.set(Transform::GetComponentId());
    Engine::OnTick(&OnTick);
  }
  
  bool PhysicsSimulator::VisitEntities(std::function<bool(const Entity&)> callback) {
    for(auto& e : tracked_) {
      if(!callback(e))
        return false;
    }
    return true;
  }

  void PhysicsSimulator::OnTick(const Tick& tick) {
    // VisitEntities([&tick](const Entity& e) {
    //   auto rigid_body = RigidBody::GetState(e);
    //   if(!rigid_body) {
    //     LOG(ERROR) << "no physics::RigidBody component found for " << e;
    //     return false;
    //   }

    //   auto transform = Transform::GetState(e);
    //   if(!transform) {
    //     LOG(ERROR) << "no physics::Transform component found for " << e;
    //     return false;
    //   }
      
    //   auto force = kNoForce;
    //   force += (*rigid_body)->mass * gravity_;
    //   (*rigid_body)->velocity += force / (*rigid_body)->mass * static_cast<float>(tick.dts);
    //   (*transform)->position += (*rigid_body)->velocity * static_cast<float>(tick.dts);
    //   return true;
    // });
  }
}
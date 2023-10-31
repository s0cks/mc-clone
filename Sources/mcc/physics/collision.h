#ifndef MCC_COLLISION_H
#define MCC_COLLISION_H

#include "mcc/gfx.h"
#include "mcc/physics/transform.h"

namespace mcc::physics {
  struct CollisionPoints {
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 normal;
    float depth;
    bool collision_;
  };

#define FOR_EACH_COLLIDER_TYPE(V) \
  V(Sphere)                       \
  V(Plane)

  class Collider {
  public:
    enum Type {
#define DEFINE_COLLIDER_TYPE(Name) k##Name##Collider,
      FOR_EACH_COLLIDER_TYPE(DEFINE_COLLIDER_TYPE)
#undef DEFINE_COLLIDER_TYPE
    };
  public:
    Collider() = default;
    virtual ~Collider() = default;
    virtual Type GetType() const = 0;
  };

  class SphereCollider : public Collider {
  private:
    glm::vec3 centroid;
    float radius;
  public:
    SphereCollider() = default;
    ~SphereCollider() override = default;
  };

  class PlaneCollider : public Collider {
  private:
    glm::vec3 normal;
    float distance;
  public:
    PlaneCollider();
    ~PlaneCollider() override = default;
  };

  class CollisionTest {
  protected:
    CollisionPoints result_;
  public:
    CollisionTest() = default;
    virtual ~CollisionTest() = default;

    CollisionPoints* GetResult() {
      return &result_;
    }
  };

  CollisionPoints TestSphereSphereCollision(SphereCollider* a, Transform* ta,
                                            SphereCollider* b, Transform* tb);
  CollisionPoints TestSpherePlaneCollision(SphereCollider* a, Transform* ta,
                                           PlaneCollider* b, Transform* tb);
}

#endif //MCC_COLLISION_H
#ifndef MCC_MOUSE_H
#define MCC_MOUSE_H

#include <vector>
#include <functional>
#include <cstdlib>

#include "mcc/murmur.h"
#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/bitfield.h"
#include "mcc/ecs/entity.h"

#include "mcc/mouse/mouse_constants.h"

namespace mcc {
  class Mouse {
  public:
    typedef uint64_t RawSubscription;
    static constexpr const RawSubscription kInvalidSubscription = 0;
    class Subscription {
    public:
      typedef uint32_t SubscriptionId;
      enum Type : uint8_t {
        kPositionType = 0,
        kButtonType = 1,
        kNumberOfTypes = 2,
      };
    private:
      template<typename T, int Pos, int Size>
      class SubscriptionField : public BitField<RawSubscription, T, Pos, Size>{};
    public:
      enum Layout {
        kTypeOffset = 0,
        kBitsForType = 8,

        kButtonOffset = kTypeOffset + kBitsForType,
        kBitsForButton = 8,
        
        kStateOffset = kButtonOffset + kBitsForButton,
        kBitsForState = 8,

        kIdOffset = kStateOffset + kBitsForState,
        kBitsForId = 32,

        kTotaBits = kBitsForType + kBitsForButton + kBitsForState + kBitsForId,
      };

      class TypeField : public SubscriptionField<Type, kTypeOffset, kBitsForType>{};
      class ButtonField : public SubscriptionField<MouseButton, kButtonOffset, kBitsForButton>{};
      class StateField : public SubscriptionField<MouseButtonState, kStateOffset, kBitsForState>{};
      class IdField : public SubscriptionField<SubscriptionId, kIdOffset, kBitsForId>{};

      static constexpr const uint64_t kMaxNumberOfPositionSubscriptions = 8;
    private:
      RawSubscription raw_;
    public:
      constexpr Subscription(const RawSubscription raw):
        raw_(raw) {
      }
      constexpr Subscription(const Subscription& rhs):
        raw_(rhs.raw_) {  
      }
      ~Subscription() = default;

      RawSubscription raw() const {
        return raw_;
      }

      SubscriptionId GetId() const {
        return IdField::Decode(raw_);
      }

      void SetId(const SubscriptionId value) {
        raw_ = IdField::Update(value, raw_);
      }

      MouseButton GetButton() const {
        return ButtonField::Decode(raw_);
      }

      void SetButton(const MouseButton value) {
        raw_ = ButtonField::Update(value, raw_);
      }

      MouseButtonState GetState() const {
        return StateField::Decode(raw_);
      }

      void SetState(const MouseButtonState value) {
        raw_ = StateField::Update(value, raw_);
      }

      Type GetType() const {
        return TypeField::Decode(raw_);
      }

      void SetType(const Type value) {
        raw_ = TypeField::Update(value, raw_);
      }

      void operator=(const RawSubscription& rhs) {
        raw_ = rhs;
      }

      constexpr operator RawSubscription () const {
        return raw_;
      }

      friend std::ostream& operator<<(std::ostream& stream, const Subscription& rhs) {
        stream << "Mouse::Subscription(";
        stream << "id=" << rhs.GetId() << ", ";
        stream << "button=" << rhs.GetButton() << ", ";
        stream << "state=" << rhs.GetState();
        stream << ")";
        return stream;
      }
    public:
      static inline constexpr Subscription
      Empty() {
        return kInvalidSubscription;
      }

      static inline constexpr Subscription
      New(const Type type) {
        return Empty() | TypeField::Encode(type);
      }

      static inline constexpr Subscription
      New(const Type type, const uint32_t id) {
        return New(type) | IdField::Encode(id);
      }

      static inline constexpr Subscription
      NewPositionSubscription(const uint32_t id) {
        return New(Type::kPositionType, id);
      }

      static inline constexpr Subscription
      NewPositionSubscription() {
        return New(Type::kPositionType);
      }

      static inline constexpr Subscription
      NewButtonSubscription(const uint32_t id, const MouseButton btn, const MouseButtonState state) {
        return New(Type::kButtonType, id) | ButtonField::Encode(btn) | StateField::Encode(state);
      }

      static inline constexpr Subscription
      NewButtonSubscription(const MouseButton btn, const MouseButtonState state) {
        return New(Type::kButtonType) | ButtonField::Encode(btn) | StateField::Encode(state);
      }
    };

    static constexpr const uint64_t kMaxNumberOfPositionSubscriptions = 8;

    static inline uint32_t
    RandomId() {
      return static_cast<uint32_t>(rand());
    }
  public:
    DEFINE_NON_INSTANTIABLE_TYPE(Mouse);
    static void Initialize(GLFWwindow* window);
    static bool IsPressed(const MouseButton button);
    static glm::vec2 GetPosition();
    static glm::vec2 GetNormalizedPosition();
    static glm::vec2 GetDelta();
    static glm::vec3 CastRay();
    static std::optional<Entity> CastRayTo(const float diff = 0.5);
    static Subscription Register(const uint32_t id, MousePositionCallback callback);

    static inline Subscription
    Register(const char* name, MousePositionCallback callback) {
      return Register(Murmur(name), callback);
    }

    static inline Subscription
    Register(MousePositionCallback callback) {
      return Register(RandomId(), callback);
    }

    static Subscription Register(const uint32_t id, const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback);

    static inline Subscription
    Register(const char* name, const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback) {
      return Register(Murmur(name), btn, state, callback);
    }

    static inline Subscription
    Register(const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback) {
      return Register(RandomId(), btn, state, callback);
    }

    static void Deregister(const Subscription& subscription);
  };
}

#endif //MCC_MOUSE_H
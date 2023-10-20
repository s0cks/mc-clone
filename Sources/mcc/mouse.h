#ifndef MCC_MOUSE_H
#define MCC_MOUSE_H

#include <vector>
#include <functional>
#include <cstdlib>

#include "mcc/murmur.h"
#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/bitfield.h"
#include "mcc/ecs/event.h"

namespace mcc {
  typedef glm::vec2 MousePosition;

  typedef std::function<void(const MousePosition& pos)> MousePositionCallback;
  typedef std::vector<MousePositionCallback> MousePositionCallbackList;

  enum MouseButtonState {
    kMousePressed = GLFW_PRESS,
    kMouseReleased = GLFW_RELEASE,
    kNumberOfMouseButtonStates = 2,
  };

  enum MouseButton {
    kMouseButton1 = GLFW_MOUSE_BUTTON_1,
    kMouseButton2 = GLFW_MOUSE_BUTTON_2,
    kNumberOfMouseButtons = 2,
  };

  typedef EventId MouseEventId;

#define FOR_EACH_MOUSE_EVENT(V) \
  V(Pressed)                    \
  V(Released)

  typedef std::function<void()> MouseButtonCallback;
  typedef std::vector<MouseButtonCallback> MouseButtonCallbackList;

  class Mouse {
  public:
    typedef uint64_t RawSubscription;
    static constexpr const RawSubscription kInvalidSubscription = 0;
    class Subscription {
    private:
      template<typename T, int Pos, int Size>
      class SubscriptionField : public BitField<RawSubscription, T, Pos, Size>{};
    public:
      enum Layout {
        kButtonOffset = 0,
        kBitsForButton = 8,
        
        kStateOffset = kButtonOffset + kBitsForButton,
        kBitsForState = 8,

        kPositionOffset = kStateOffset + kBitsForState,
        kBitsForPosition = 1,

        kIdOffset = kStateOffset + kBitsForState,
        kBitsForId = 32,

        kTotaBits = kBitsForButton + kBitsForState + kBitsForId,
      };

      class ButtonField : public SubscriptionField<MouseButton, kButtonOffset, kBitsForButton>{};
      class StateField : public SubscriptionField<MouseButtonState, kStateOffset, kBitsForState>{};
      class PositionField : public SubscriptionField<bool, kPositionOffset, kBitsForPosition>{};
      class IdField : public SubscriptionField<uint32_t, kIdOffset, kBitsForId>{};

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

      uint32_t GetId() const {
        return IdField::Decode(raw_);
      }

      void SetId(const uint32_t value) {
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

      bool IsPosition() const {
        return PositionField::Decode(raw_);
      }

      void SetPosition(const bool value) {
        raw_ = PositionField::Update(value, raw_);
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
      Id(const uint32_t id) {
        return Empty() | IdField::Encode(id);
      }

      static inline constexpr Subscription
      NewPositionSubscription(const uint32_t id) {
        return Id(id) | PositionField::Encode(true);
      }

      static inline constexpr Subscription
      NewButtonSubscription(const uint32_t id, const MouseButton btn, const MouseButtonState state) {
        return Id(id) | ButtonField::Encode(btn) | StateField::Encode(state);
      }

      static inline constexpr Subscription
      NewButtonSubscription(const MouseButton btn, const MouseButtonState state) {
        return Empty() | ButtonField::Encode(btn) | StateField::Encode(state);
      }
    };

    static constexpr const uint64_t kMaxNumberOfPositionSubscriptions = 8;
  private:
#define DECLARE_SEND_EVENT(Name) \
    static void Send##Name##Event();
    FOR_EACH_MOUSE_EVENT(DECLARE_SEND_EVENT)
#undef DECLARE_SEND_EVENT
  public:
    DEFINE_NON_INSTANTIABLE_TYPE(Mouse);
    static void Initialize(GLFWwindow* window);
    static bool IsPressed(const MouseButton button);
    static glm::vec2 GetPosition();
    static glm::vec2 GetDelta();
    static Subscription Register(MousePositionCallback callback);
    static Subscription Register(const uint32_t id, const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback);

    static inline Subscription
    Register(const char* name, const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback) {
      return Register(Murmur(name), btn, state, callback);
    }

    static inline Subscription
    Register(const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback) {
      return Register(static_cast<uint32_t>(rand()), btn, state, callback);
    }

    static void Deregister(const Subscription& subscription);

#define DECLARE_REGISTER_LISTENER(Name) \
    static void Register##Name##EventListener(EventListener listener);
    FOR_EACH_MOUSE_EVENT(DECLARE_REGISTER_LISTENER)
#undef DECLARE_REGISTER_LISTENER
  };
}

#endif //MCC_MOUSE_H
#ifndef MCC_KEYBOARD_H
#define MCC_KEYBOARD_H

#include "mcc/gfx.h"
#include "mcc/common.h"
#include "mcc/uv_utils.h"
#include "mcc/bitfield.h"
#include "mcc/keyboard/keyboard_constants.h"

namespace mcc::keyboard {
  class Keyboard {
    DEFINE_NON_INSTANTIABLE_TYPE(Keyboard);
  public:
    typedef uint64_t RawSubscription;
    static constexpr const RawSubscription kInvalidSubscription = 0;
    class Subscription {
    public:
      typedef uint16_t SubscriptionId;
    private:
      template<typename T, int Pos, int Size>
      class SubscriptionField : public BitField<RawSubscription, T, Pos, Size>{};
    public:
      enum Layout {
        kIdOffset = 0,
        kBitsForId = 16,

        kStateOffset = kIdOffset + kBitsForId,
        kBitsForState = 8,
        
        kKeyCodeOffset = kStateOffset + kBitsForState,
        kBitsForKeyCode = 32,

        kTotalBits = kBitsForId + kBitsForState + kBitsForKeyCode,
      };

      class IdField : public SubscriptionField<SubscriptionId, kIdOffset, kBitsForId>{};
      class StateField : public SubscriptionField<KeyState, kStateOffset, kBitsForState>{};
      class KeyCodeField : public SubscriptionField<KeyCode, kKeyCodeOffset, kBitsForKeyCode>{};
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

      KeyState GetState() const {
        return StateField::Decode(raw_);
      }

      void SetState(const KeyState value) {
        raw_ = StateField::Update(value, raw_);
      }

      KeyCode GetKeyCode() const {
        return KeyCodeField::Decode(raw_);
      }

      void SetKeyCode(const KeyCode value) {
        raw_ = KeyCodeField::Update(value, raw_);
      }

      void operator=(const RawSubscription& rhs) {
        raw_ = rhs;
      }

      constexpr operator RawSubscription () const {
        return raw_;
      }

      friend std::ostream& operator<<(std::ostream& stream, const Subscription& rhs) {
        stream << "Keyboard::Subscription(";
        stream << "id=" << rhs.GetId() << ", ";
        stream << "code=" << rhs.GetKeyCode() << ", ";
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
      New(const SubscriptionId id, const KeyCode code, const KeyState state) {
        return Empty() | IdField::Encode(id) | KeyCodeField::Encode(code) | StateField::Encode(state);
      }

      static inline constexpr Subscription
      New(const KeyCode code, const KeyState state) {
        return Empty() | KeyCodeField::Encode(code) | StateField::Encode(state);
      }
    };

    static inline Subscription::SubscriptionId
    RandomSubscriptionId() {
      return static_cast<Subscription::SubscriptionId>(rand());
    }
  public:
    static void Initialize();
    static void Deregister(const Subscription& subscription);
    static Subscription Register(const Subscription::SubscriptionId id, const KeyCode code, const KeyState state, KeyCallback callback);

    static inline Subscription
    Register(const KeyCode key, const KeyState state, KeyCallback callback) {
      return Register(RandomSubscriptionId(), key, state, callback);
    }
  };
}

#endif //MCC_KEYBOARD_H
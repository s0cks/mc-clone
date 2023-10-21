#ifndef MCC_KEYBOARD_TRIE_H
#define MCC_KEYBOARD_TRIE_H

#include "mcc/keyboard/keyboard.h"
#include "mcc/subscription_trie.h"

namespace mcc::keyboard {
#define FOR_EACH_KEYBOARD_TRIE_NODE(V) \
  V(Root) \
  V(KeyCode) \
  V(KeyState) \
  V(KeySubscription)

#define FORWARD_DECLARE_NODE(Name) \
  class Name##Node;
  FOR_EACH_KEYBOARD_TRIE_NODE(FORWARD_DECLARE_NODE)
#undef FORWARD_DECLARE_NODE

  class KeySubscriptionNode : public trie::SubscriptionNode<Keyboard::Subscription, KeyCallback> {
  public:
    KeySubscriptionNode(const Keyboard::Subscription& subscription, KeyCallback callback):
      SubscriptionNode(subscription, callback) {
    }
    ~KeySubscriptionNode() override = default;

    void Call(const Keyboard::Subscription& subscription) {
      return callback_();
    }

    friend std::ostream& operator<<(std::ostream& stream, const KeySubscriptionNode& rhs) {
      stream << "KeySubscriptionNode(";
      stream << "subscription=" << rhs.subscription_;
      stream << ")";
      return stream;
    }
  };

  class KeyStateNode : public trie::SubscriptionSequenceNode<Keyboard::Subscription, KeyCallback> {
  protected:
    KeyState state_;
  public:
    KeyStateNode(const KeyState state):
      state_(state) {
    }
    ~KeyStateNode() override = default;

    KeyState GetState() const {
      return state_;
    }

    void Call(const Keyboard::Subscription& subscription) {
      SequenceNodeIterator iter(this);
      while(iter.HasNext()) {
        const auto next = (KeySubscriptionNode*)iter.Next();
        next->Call(subscription);
      }
    }
  };

  class KeyCodeNode : public trie::EdgeNode<kNumberOfKeyStates> {
  private:
    KeyCode code_;
  public:
    KeyCodeNode(const KeyCode code):
      EdgeNode(),
      code_(code) {
      for(auto idx = 0; idx < kNumberOfKeyStates; idx++) {
        children_[idx] = new KeyStateNode(static_cast<KeyState>(idx));
      }
    }
    ~KeyCodeNode() override = default;

    KeyCode GetKeyCode() const {
      return code_;
    }

    KeyStateNode* GetStateAt(const KeyState state) {
      return (KeyStateNode*) GetChildAt(state);
    }

    KeyStateNode* GetStateAt(const Keyboard::Subscription& subscription) {
      return GetStateAt(subscription.GetState());
    }

    void Call(const Keyboard::Subscription& subscription) {
      return GetStateAt(subscription)->Call(subscription);
    }

    void Register(const Keyboard::Subscription& subscription, KeyCallback callback) {
      return GetStateAt(subscription)->Register(subscription, callback);
    }

    void Deregister(const Keyboard::Subscription& subscription) {
      return GetStateAt(subscription)->Deregister(subscription);
    }
  };

  class RootNode : public trie::EdgeNode<kNumberOfKeyCodes> {
  public:
    RootNode():
      EdgeNode() {
      for(auto idx = 0; idx < kNumberOfKeyCodes; idx++)
        children_[idx] = new KeyCodeNode(static_cast<KeyCode>(idx));
    }
    ~RootNode() override = default;

    KeyCodeNode* GetKeyAt(const KeyCode idx) {
      return (KeyCodeNode*) GetChildAt(idx);
    }

    KeyCodeNode* GetKeyAt(const Keyboard::Subscription& subscription) {
      return GetKeyAt(subscription.GetKeyCode());
    }

    void Call(const Keyboard::Subscription& subscription) {
      return GetKeyAt(subscription)->Call(subscription);
    }

    void Register(const Keyboard::Subscription& subscription, KeyCallback callback) {
      return GetKeyAt(subscription)->Register(subscription, callback);
    }

    void Deregister(const Keyboard::Subscription& subscription) {
      return GetKeyAt(subscription)->Deregister(subscription);
    }
  };
}

#endif //MCC_KEYBOARD_TRIE_H
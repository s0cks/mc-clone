#ifndef MCC_MOUSE_TRIE_H
#define MCC_MOUSE_TRIE_H

#include "mcc/mouse/mouse.h"
#include "mcc/subscription_trie.h"

namespace mcc {
#define FOR_EACH_MOUSE_TRIE_NODE(V) \
  V(Root)                           \
  V(MousePosition)                  \
  V(MousePositionSubscription)      \
  V(MouseButton)                    \
  V(MouseButtonState)               \
  V(MouseButtonSubscription)

#define FORWARD_DECLARE_NODE(Name) \
  class Name##Node;
  FOR_EACH_MOUSE_TRIE_NODE(FORWARD_DECLARE_NODE)
#undef FORWARD_DECLARE_NODE

  class MouseButtonStateNode;
  class MouseButtonSubscriptionNode : public trie::SubscriptionNode<Mouse::Subscription, MouseButtonCallback> {
  public:
    MouseButtonSubscriptionNode(const Mouse::Subscription& subscription, MouseButtonCallback callback):
      SubscriptionNode(subscription, callback) {
    }
    ~MouseButtonSubscriptionNode() override = default;

    void Call(const Mouse::Subscription& subscription) {
      return callback_();
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonSubscriptionNode& rhs) {
      stream << "MouseButtonSubscriptionNode(";
      stream << "subscription=" << rhs.subscription_;
      return stream << ")";
    }
  };

  class MouseButtonStateNode : public trie::SubscriptionSequenceNode<Mouse::Subscription, MouseButtonCallback> {
  protected:
    MouseButtonState state_;
  public:
    MouseButtonStateNode(const MouseButtonState state):
      SubscriptionSequenceNode(),
      state_(state) {
    }
    ~MouseButtonStateNode() override = default;

    MouseButtonState GetState() const {
      return state_;
    }

    void Call(const Mouse::Subscription& subscription) {
      SequenceNodeIterator iter(this);
      while(iter.HasNext()) {
        const auto next = (MouseButtonSubscriptionNode*)iter.Next();
        next->Call(subscription);
      }
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonStateNode& rhs) {
      stream << "MouseButtonStateNode(";
      stream << "state=" << rhs.GetState();
      return stream << ")";
    }
  };

  template<const uint64_t NumberOfChildren>
  class SubscriptionEdgeNode : public trie::EdgeNode<NumberOfChildren> {
  public:
    virtual void Call(const Mouse::Subscription& subscription) = 0;
    virtual void Deregister(const Mouse::Subscription& subscription) = 0;
  };

  class MouseButtonNode : public SubscriptionEdgeNode<kNumberOfMouseButtonStates> {
  protected:
    MouseButton button_;
  public:
    explicit MouseButtonNode(const MouseButton button):
      SubscriptionEdgeNode<kNumberOfMouseButtonStates>(),
      button_(button) {
      for(auto idx = 0; idx < kNumberOfMouseButtonStates; idx++) {
        children_[idx] = new MouseButtonStateNode(static_cast<MouseButtonState>(idx));
      }
    }
    ~MouseButtonNode() override = default;

    MouseButton GetButton() const {
      return button_;
    }

    inline MouseButtonStateNode* GetButtonStateNode(const MouseButtonState state) const {
      return (MouseButtonStateNode*) children_[state];
    }

    inline MouseButtonStateNode* GetButtonStateNode(const Mouse::Subscription& subscription) const {
      return GetButtonStateNode(subscription.GetState());
    }

    void Call(const Mouse::Subscription& subscription) override {
      return GetButtonStateNode(subscription)->Call(subscription);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      return GetButtonStateNode(subscription)->Register(subscription, callback);
    }

    virtual void Deregister(const Mouse::Subscription& subscription) override {
      return GetButtonStateNode(subscription)->Deregister(subscription);
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonNode& rhs) {
      stream << "MouseButtonNode(";
      stream << "button=" << rhs.GetButton();
      return stream << ")";
    }
  };

  class MouseButtonsNode : public SubscriptionEdgeNode<kNumberOfMouseButtons> {
  public:
    explicit MouseButtonsNode():
      SubscriptionEdgeNode<kNumberOfMouseButtons>() {
      for(auto idx = 0; idx < kNumberOfMouseButtons; idx++)
        children_[idx] = new MouseButtonNode(static_cast<MouseButton>(idx));
    }
    ~MouseButtonsNode() override = default;

    inline MouseButtonNode* GetMouseButtonNode(const MouseButton idx) const {
      return (MouseButtonNode*) children_[idx];
    }

    inline MouseButtonNode* GetMouseButtonNode(const Mouse::Subscription& subscription) const {
      return GetMouseButtonNode(subscription.GetButton());
    }

    void Call(const Mouse::Subscription& subscription) override {
      return GetMouseButtonNode(subscription)->Call(subscription);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      return GetMouseButtonNode(subscription)->Register(subscription, callback);
    }

    void Deregister(const Mouse::Subscription& subscription) override {
      return GetMouseButtonNode(subscription)->Deregister(subscription);
    }
  };

  class MousePositionsNode;
  class MousePositionSubscriptionNode : public trie::SubscriptionNode<Mouse::Subscription, MousePositionCallback> {
  public:
    MousePositionSubscriptionNode(const Mouse::Subscription& subscription, MousePositionCallback callback):
      SubscriptionNode(subscription, callback) {
    }
    ~MousePositionSubscriptionNode() override = default;

    void Call(const Mouse::Subscription& subscription, const glm::vec2 pos) {
      return callback_(pos);
    }

    friend std::ostream& operator<<(std::ostream& stream, const MousePositionSubscriptionNode& rhs) {
      stream << "MouseButtonStateSubscriptionNode(";
      stream << "subscription=" << rhs.subscription_;
      return stream << ")";
    }
  };

  class MousePositionsNode : public trie::SubscriptionSequenceNode<Mouse::Subscription, MousePositionCallback> {
  public:
    explicit MousePositionsNode():
      SubscriptionSequenceNode() {
    }
    ~MousePositionsNode() override = default;

    void Call(const Mouse::Subscription& subscription, const glm::vec2 pos) {
      SequenceNodeIterator iter(this);
      while(iter.HasNext()) {
        const auto next = (MousePositionSubscriptionNode*)iter.Next();
        next->Call(subscription, pos);
      }
    }
  };

  class RootNode : public trie::EdgeNode<2> {
  public:
    static constexpr const int kButtonsNodeIndex = 1;
    static constexpr const int kPositionNodeIndex = 0;
  public:
    RootNode():
      trie::EdgeNode<2>() {
      children_[kPositionNodeIndex] = new MousePositionsNode();
      children_[kButtonsNodeIndex] = new MouseButtonsNode();
    }
    ~RootNode() override = default;

    inline MouseButtonsNode* GetButtonsNode() const {
      return (MouseButtonsNode*) children_[kButtonsNodeIndex];
    }

    inline MousePositionsNode* GetPositionsNode() const {
      return (MousePositionsNode*) children_[kPositionNodeIndex];
    }

    void Call(const Mouse::Subscription& subscription) {
      return GetButtonsNode()->Call(subscription);
    }

    void Call(const Mouse::Subscription& subscription, const glm::vec2 pos) {
      return GetPositionsNode()->Call(subscription, pos);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      return GetButtonsNode()->Register(subscription, callback);
    }

    void Register(const Mouse::Subscription& subscription, MousePositionCallback callback) {
      return GetPositionsNode()->Register(subscription, callback);
    }

    void Deregister(const Mouse::Subscription& subscription) {
      return GetPositionsNode()->Deregister(subscription);
    }

    friend std::ostream& operator<<(std::ostream& stream, const RootNode& rhs) {
      stream << "RootNode()";
      return stream;
    }
  };
}

#endif //MCC_MOUSE_TRIE_H
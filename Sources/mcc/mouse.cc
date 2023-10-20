#include "mcc/mouse.h"

namespace mcc {
  class Node {
  protected:
    Node* parent_;

    explicit Node(Node* parent):
      parent_(parent) {
    }
  public:
    virtual ~Node() = default;

    Node* GetParent() const {
      return parent_;
    }

    void SetParent(Node* parent) {
      parent_ = parent;
    }

    bool HasParent() const {
      return parent_ != nullptr;
    }

    virtual bool IsLeaf() const {
      return false;
    }

    virtual uint64_t GetNumberOfChildren() const {
      return 0;
    }

    virtual void Call(const Mouse::Subscription& subscription) = 0;
  };

  template<const uint64_t NumberOfChildren>
  class EdgeNode : public Node {
  protected:
    Node* children_[NumberOfChildren];

    explicit EdgeNode(Node* parent):
      Node(parent) {
    }
  public:
    ~EdgeNode() override = default;

    bool IsLeaf() const override {
      return NumberOfChildren == 0;
    }
    
    uint64_t GetNumberOfChildren() const override {
      return NumberOfChildren;
    }

    virtual Node* GetChildAt(const uint64_t idx) const {
      return children_[idx];
    }
  };

  class SubscriptionNode : public Node {
  protected:
    Mouse::Subscription subscription_;
    SubscriptionNode* next_;
    SubscriptionNode* previous_;

    explicit SubscriptionNode(Node* parent, const Mouse::Subscription& subscription):
      Node(parent),
      subscription_(subscription),
      next_(nullptr),
      previous_(nullptr) {
    }
  public:
    ~SubscriptionNode() override = default;

    Mouse::Subscription GetSubscription() const {
      return subscription_;
    }

    SubscriptionNode* GetNext() const {
      return next_;
    }

    void SetNext(SubscriptionNode* node) {
      next_ = node;
    }

    bool HasNext() const {
      return next_;
    }

    SubscriptionNode* GetPrevious() const {
      return previous_;
    }

    void SetPrevious(SubscriptionNode* node) {
      previous_ = node;
    }

    bool HasPrevious() const {
      return previous_ != nullptr;
    }

    friend bool operator==(const SubscriptionNode& lhs, const Mouse::Subscription& rhs) {
      return lhs.subscription_ == rhs;
    }

    friend bool operator!=(const SubscriptionNode& lhs, const Mouse::Subscription& rhs) {
      return lhs.subscription_ != rhs;
    }
  };

  template<class C>
  class SubscriptionNodeTemplate : public SubscriptionNode {
  protected:
    C callback_;
  public:
    SubscriptionNodeTemplate(Node* parent, const Mouse::Subscription& subscription, C callback):
      SubscriptionNode(parent, subscription),
      callback_(callback) {
    }
    ~SubscriptionNodeTemplate() override = default;

    void Call(const Mouse::Subscription& subscription) override {
      callback_();
    }
  };

  template<typename C>
  class SubscriptionListNode : public Node {
  protected:
    SubscriptionNode* head_;
    SubscriptionNode* tail_;

    explicit SubscriptionListNode(Node* parent):
      Node(parent) {
    }
  public:
    ~SubscriptionListNode() override = default;

    SubscriptionNode* GetHead() const {
      return head_;
    }

    void SetHead(SubscriptionNode* node) {
      head_ = node;
    }

    bool HasHead() const {
      return head_ != nullptr;
    }

    SubscriptionNode* GetTail() const {
      return tail_;
    }

    void SetTail(SubscriptionNode* node) {
      tail_ = node;
    }

    bool HasTail() const {
      return tail_ != nullptr;
    }


    void Call(const Mouse::Subscription& subscription) override {
      auto node = head_;
      while(node != nullptr) {
        node->Call(subscription);
        node = node->GetNext();
      }
    }

    void Register(const Mouse::Subscription& subscription, C callback) {
      const auto new_node = new SubscriptionNodeTemplate(this, subscription, callback);
      new_node->SetPrevious(tail_);
      if(tail_ != nullptr)
        tail_->SetNext(new_node);
      tail_ = new_node;
      if(head_ == nullptr)
        head_ = new_node;
    }

    void Deregister(const Mouse::Subscription& subscription) {
      if(head_ == nullptr)
        return;

      if((*head_) == subscription) {
        const auto next = head_->GetNext();
        delete head_;
        head_ = next;
        return;
      }

      auto node = head_->GetNext();
      while(node != nullptr) {
        if((*node) == subscription)
          break;
        node = node->GetNext();
      }

      if(node == nullptr)
        return;
      const auto prev = node->GetPrevious();
      const auto next = node->GetNext();
      if(prev)
        prev->SetNext(next);
      if(next)
        next->SetPrevious(prev);
      delete node;
    }
  };

  class MouseButtonStateNode;
  class MouseButtonStateSubscriptionNode : public SubscriptionNodeTemplate<MouseButtonCallback> {
  public:
    MouseButtonStateSubscriptionNode(Node* parent, const Mouse::Subscription& subscription, MouseButtonCallback callback):
      SubscriptionNodeTemplate(parent, subscription, callback) {
    }
    ~MouseButtonStateSubscriptionNode() override = default;

    void Call(const Mouse::Subscription& subscription) override {
      DLOG(INFO) << (*this) << "calling " << subscription;
      return callback_();
    }

    friend bool operator==(const MouseButtonStateSubscriptionNode& lhs, const Mouse::Subscription& rhs) {
      return lhs.subscription_ == rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonStateSubscriptionNode& rhs) {
      stream << "MouseButtonStateSubscriptionNode(";
      stream << "subscription=" << rhs.subscription_;
      return stream << ")";
    }
  };

  class MouseButtonStateNode : public SubscriptionListNode<MouseButtonCallback> {
  protected:
    MouseButtonState state_;
  public:
    MouseButtonStateNode(Node* parent, const MouseButtonState state):
      SubscriptionListNode(parent),
      state_(state) {
    }
    ~MouseButtonStateNode() override = default;

    MouseButtonState GetState() const {
      return state_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonStateNode& rhs) {
      stream << "MouseButtonStateNode(";
      stream << "state=" << rhs.GetState();
      return stream << ")";
    }
  };

  class MouseButtonNode : public EdgeNode<kNumberOfMouseButtonStates> {
  protected:
    MouseButton button_;
  public:
    explicit MouseButtonNode(Node* parent, const MouseButton button):
      EdgeNode(parent),
      button_(button) {
      for(auto idx = 0; idx < kNumberOfMouseButtonStates; idx++) {
        children_[idx] = new MouseButtonStateNode(this, static_cast<MouseButtonState>(idx));
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
      DLOG(INFO) << (*this) << " calling " << subscription;
      return GetButtonStateNode(subscription)->Call(subscription);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      DLOG(INFO) << (*this) << "registering " << subscription;
      return GetButtonStateNode(subscription)->Register(subscription, callback);
    }

    void Deregister(const Mouse::Subscription& subscription) {
      DLOG(INFO) << (*this) << "deregistering " << subscription;
      return GetButtonStateNode(subscription)->Deregister(subscription);
    }

    friend std::ostream& operator<<(std::ostream& stream, const MouseButtonNode& rhs) {
      stream << "MouseButtonNode(";
      stream << "button=" << rhs.GetButton();
      return stream << ")";
    }
  };

  class MouseButtonsNode : public EdgeNode<kNumberOfMouseButtons> {
  public:
    explicit MouseButtonsNode(Node* parent):
      EdgeNode(parent) {
      for(auto idx = 0; idx < kNumberOfMouseButtons; idx++)
        children_[idx] = new MouseButtonNode(this, static_cast<MouseButton>(idx));
    }
    ~MouseButtonsNode() override = default;

    inline MouseButtonNode* GetMouseButtonNode(const MouseButton idx) const {
      return (MouseButtonNode*)children_[idx];
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

    void Deregister(const Mouse::Subscription& subscription) {
      return GetMouseButtonNode(subscription)->Deregister(subscription);
    }
  };

  class MousePositionsNode : public SubscriptionListNode<MousePositionCallback> {
  public:
    explicit MousePositionsNode(Node* parent):
      SubscriptionListNode<MousePositionCallback>(parent) {
    }
    ~MousePositionsNode() override = default;
  };

  class RootNode : public EdgeNode<2> {
  public:
    static constexpr const int kButtonsNodeIndex = 1;
    static constexpr const int kPositionNodeIndex = 0;
  public:
    RootNode():
      EdgeNode(nullptr) {
      children_[kPositionNodeIndex] = new MousePositionsNode(this);
      children_[kButtonsNodeIndex] = new MouseButtonsNode(this);
    }
    ~RootNode() override = default;

    inline MouseButtonsNode* GetButtonsNode() const {
      return (MouseButtonsNode*) children_[kButtonsNodeIndex];
    }

    inline MousePositionsNode* GetPositionsNode() const {
      return (MousePositionsNode*) children_[kPositionNodeIndex];
    }

    void Call(const Mouse::Subscription& subscription) override {
      return subscription.IsPosition()
           ? GetPositionsNode()->Call(subscription)
           : GetButtonsNode()->Call(subscription);
    }

    void Register(const Mouse::Subscription& subscription, MouseButtonCallback callback) {
      return GetButtonsNode()->Register(subscription, callback);
    }

    void Deregister(const Mouse::Subscription& subscription) {
      return subscription.IsPosition() 
          ? GetPositionsNode()->Deregister(subscription)
          : GetButtonsNode()->Deregister(subscription);
    }

    friend std::ostream& operator<<(std::ostream& stream, const RootNode& rhs) {
      stream << "RootNode()";
      return stream;
    }
  };

  static RootNode* root_;

  static inline void
  Call(const Mouse::Subscription subscription) {
    DLOG(INFO) << "calling " << subscription;
    if(root_ != nullptr)
      return root_->Call(subscription);
  }

  static MousePositionCallbackList pos_listeners_;
  static bool state[] = {
    false,
    false,
  };
  static glm::vec2 pos = glm::vec2(0,0);
  static glm::vec2 delta = glm::vec2(0, 0);

  static const MouseEventId kMousePressedEventId = Murmur("Event::Mouse::Pressed");
  static const MouseEventId kMouseReleasedEventId = Murmur("Event::Mouse::Released");

  static inline void 
  OnMouseInput(GLFWwindow* window, int btn, int action, int mods) {
    if(action == kMousePressed) {
      state[btn] = true;
    } else {
      state[btn] = false;
    }
    const auto subscription = Mouse::Subscription::NewButtonSubscription(static_cast<MouseButton>(btn), static_cast<MouseButtonState>(action));
    Call(subscription);
  }

  template<typename T>
  static inline T
  Max(const T lhs, const T rhs) {
    return lhs > rhs ? lhs : rhs;
  }

  template<typename T>
  static inline T
  Min(const T lhs, const T rhs) {
    return lhs < rhs ? lhs : rhs;
  }

  template<typename T>
  static inline T
  Clamp(const T value, const T min, const T max) {
    return Max(min, Min(max, value));
  }

  static inline void
  OnCursor(GLFWwindow* window, double x, double y) {
    const auto p = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    delta = p - pos;
    delta = glm::vec2(Clamp(delta[0], -100.0f, 100.0f), Clamp(delta[1], -100.0f, 100.0f));
    pos = p;
  }

  void Mouse::Initialize(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, &OnMouseInput);
    glfwSetCursorPosCallback(window, &OnCursor);
    root_ = new RootNode();
  }

  Mouse::Subscription Mouse::Register(MousePositionCallback callback) {
    pos_listeners_.push_back(callback);
    return kInvalidSubscription; //TODO: implement
  }

  Mouse::Subscription Mouse::Register(const uint32_t id, const MouseButton btn, const MouseButtonState state, MouseButtonCallback callback) {
    const auto subscription = Subscription::NewButtonSubscription(id, btn, state);
    root_->Register(subscription, callback);
    return subscription;
  }

  void Mouse::Deregister(const Subscription& subscription) {
    DLOG(INFO) << "deregistering " << subscription << "....";
    return root_->Deregister(subscription);
  }

  bool Mouse::IsPressed(const MouseButton button) {
    return state[button];
  }

  glm::vec2 Mouse::GetPosition() {
    return glm::vec2(pos);
  }

  glm::vec2 Mouse::GetDelta() {
    return glm::vec2(delta);
  }

#define DEFINE_SEND_EVENT(Name) \
  void Mouse::Send##Name##Event() { Events::Send(kMouse##Name##EventId); }
  FOR_EACH_MOUSE_EVENT(DEFINE_SEND_EVENT)
#undef DEFINE_SEND_EVENT

#define DEFINE_REGISTER_LISTENER(Name) \
  void Mouse::Register##Name##EventListener(EventListener listener) { Events::Register(kMouse##Name##EventId, listener); }
  FOR_EACH_MOUSE_EVENT(DEFINE_REGISTER_LISTENER)
#undef DEFINE_REGISTER_LISTENER
}
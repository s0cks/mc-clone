#ifndef MCC_EVENT_BUS_H
#define MCC_EVENT_BUS_H

#include <uv.h>
#include <string>
#include <functional>
#include <glog/logging.h>

#include "mcc/trie.h"
#include "mcc/platform.h"

namespace mcc {
  namespace event {
    static constexpr const char* kEventBusAlphabet =
      "abcdefghijklmnopqrstuvwxyz:./-_+=&$%";
    static constexpr const uint64_t kEventBusAlphabetSize = 37;

    template<typename Event>
    class EventBusCallback : public trie::Node {
      friend class EventBus;
    public:
      typedef std::function<void(const Event&)> Callback;
    protected:
      EventBusCallback* next_;
      EventBusCallback* previous_;
      Callback callback_;

      explicit EventBusCallback(Callback callback):
        Node(),
        next_(nullptr),
        previous_(nullptr),
        callback_(callback) {
      }
    public:
      ~EventBusCallback() override = default;

      EventBusCallback* GetNext() const {
        return next_;
      }

      void SetNext(EventBusCallback* next) {
        next_ = next;
      }

      bool HasNext() const {
        return next_ != nullptr;
      }

      EventBusCallback* GetPrevious() const {
        return previous_ != nullptr;
      }

      void SetPrevious(EventBusCallback* previous) {
        previous_ = previous;
      }

      bool HasPrevious() const {
        return previous_;
      }

      void operator()(const Event& event) {
        return callback_(event);
      }
    };

    using trie::Node;
    using trie::EdgeNode;

    class EventBusNode : public EdgeNode<kEventBusAlphabetSize + 1> {
      friend class EventBus;
    protected:
      char key_;

      EventBusNode():
        EdgeNode(),
        key_() {
      }
      EventBusNode(const char key):
        EdgeNode(),
        key_(key) {
      }
    public:
      ~EventBusNode() override = default;
    };

    class EventBus {
    public:
      typedef uint64_t Subscription;
    private:
      Node* root_;

      bool Insert(Node* root, const std::string& path, Node* node) {
        Node* curr = root;
        for(auto idx = 0; idx < path.length(); idx++) {
          const auto pos = (int) (strchr(kEventBusAlphabet, tolower(path[idx])) - kEventBusAlphabet);
          if(!curr->HasChildAt(pos))
            curr->SetChildAt(pos, new EventBusNode(pos));
          curr = curr->GetChildAt(pos);
        }
        curr->SetChildAt(kEventBusAlphabetSize + 1, node);
        return true;
      }

      Node* Search(Node* root, const std::string& path) const {
        Node* curr = root;
        for(auto idx = 0; idx < path.length() && curr != nullptr; idx++) {
          const auto pos = (int) (strchr(kEventBusAlphabet, tolower(path[idx])) - kEventBusAlphabet);
          curr = curr->GetChildAt(pos);
        }
        return curr->GetChildAt(kEventBusAlphabetSize + 1);
      }
    public:
      EventBus():
        root_(new EventBusNode()) {
      }
      EventBus(const EventBus& rhs):
        root_(rhs.root_) {
      }
      virtual ~EventBus() = default;

      template<typename Event>
      void Register(const std::string& event, std::function<void(const Event&)> callback) {
        Insert(root_, event, new EventBusCallback<Event>(callback));
      }

      template<typename Event>
      void Register(std::function<void(const Event&)> callback) {
        return Register<Event>(typeid(Event).name(), callback);
      }

      template<typename Event>
      void Publish(const std::string& event, const Event& data) {
        const auto callback = (EventBusCallback<Event>*) Search(root_, event);
        if(callback != nullptr)
          (*callback)(data);
      }

      template<typename Event>
      void Publish(const Event& data) {
        return Publish<Event>(typeid(Event).name(), data);
      }

      void operator=(const EventBus& rhs) {
        root_ = rhs.root_;
      }
    };
  }
  using event::EventBus;
}

#endif //MCC_EVENT_BUS_H
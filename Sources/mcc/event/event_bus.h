#ifndef MCC_EVENT_BUS_H
#define MCC_EVENT_BUS_H

#include <uv.h>
#include <string>
#include <functional>

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
      Callback callback_;

      EventBusCallback(Callback callback):
        Node(),
        callback_(callback) {
      }
    public:
      ~EventBusCallback() override = default;

      void operator()(const Event& event) {
        return callback_(event);
      }
    };

    using trie::Node;
    using trie::EdgeNode;

    class EventBusNode : public EdgeNode<kEventBusAlphabetSize> {
      friend class EventBus;
    protected:
      EventBusNode() = default;
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
        for(auto idx = 0; idx < path.length() - 1; idx++) {
          const auto pos = path[idx];
          if(!curr->HasChildAt(pos))
            curr->SetChildAt(pos, new EventBusNode());
          curr = curr->GetChildAt(pos);
        }
        curr->SetChildAt(path[path.length()], node);
        return true;
      }

      Node* Search(Node* root, const std::string& path) const {
        Node* curr = root;
        for(auto idx = 0; idx < path.length(); idx++)
          curr = curr->GetChildAt(idx);
        return curr;
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
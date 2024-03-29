#include "mcc/event/event_bus.h"

#include "mcc/common.h"

namespace mcc {
  namespace event {
    class EventTrie {
      DEFINE_NON_COPYABLE_TYPE(EventTrie);
      static constexpr const auto kAlphabetSize = 128;
    private:
      class Node : public trie::EdgeNode<kAlphabetSize> {
      private:
        std::string path_;
      public:
        Node() = default;
        ~Node() override = default;

        std::string path() const {
          return path_;
        }
      };

#define FOR_EACH_SEARCH_RESULT_STATUS(V) \
  V(Ok)                                  \
  V(NotFound)

      class SearchResult {
      public:
        enum Status : uint8_t {
          kUnknownStatus = 0,
#define DEFINE_SEARCH_RESULT_STATUS(Name) k##Name##Status,
          FOR_EACH_SEARCH_RESULT_STATUS(DEFINE_SEARCH_RESULT_STATUS)
#undef DEFINE_SEARCH_RESULT_STATUS
        };

        friend std::ostream& operator<<(std::ostream& stream, const Status& rhs) {
          switch(rhs) {
#define DEFINE_TOSTRING(Name) \
            case Status::k##Name##Status: return stream << #Name;
            FOR_EACH_SEARCH_RESULT_STATUS(DEFINE_TOSTRING)
#undef DEFINE_TOSTRING
            case kUnknownStatus:
            default:
              return stream << "<unknown EventTrie::Status:  " << static_cast<uint8_t>(rhs) << ">";
          }
        }
      private:
        Status status_;
        std::string path_;
      public:
        SearchResult():
          status_(kUnknownStatus),
          path_() {
        }
        SearchResult(const Status status, const std::string& path):
          status_(status),
          path_(path) {
        }
        SearchResult(const SearchResult& rhs) = default;
        ~SearchResult() = default;
        SearchResult& operator=(const SearchResult& rhs) = default;

        Status status() const {
          return status_;
        }

        std::string path() const {
          return path_;
        }

#define DEFINE_STATUS_CHECK(Name) \
        inline bool Is##Name() const { return status() == Status::k##Name##Status; }
        FOR_EACH_SEARCH_RESULT_STATUS(DEFINE_STATUS_CHECK)
#undef DEFINE_STATUS_CHECK

        friend std::ostream& operator<<(std::ostream& stream, const SearchResult& rhs) {
          stream << "EventTrie::SearchResult(";
          stream << "status=" << rhs.status() << ", ";
          stream << "path=" << rhs.path();
          stream << ")";
          return stream;
        }
      };

      Node* root_;

      static inline bool
      Insert(Node* root, const std::string& path, Node* node) {
        MCC_ASSERT(root != nullptr);
        MCC_ASSERT(!path.empty());
        auto curr = root;
        for(auto i = path.begin(); i < path.end();) {
          auto c = *(i++);
          switch(c) {
            default: {
              const auto pos = c - 32;
              if(pos < 0) {
                DLOG(WARNING) << "invalid character: " << c << "(" << pos << ")";
                return false;
              }

              if(!curr->HasChildAt(pos)) {
                if(!curr->SetChildAt(pos, node)) {
                  DLOG(WARNING) << "couldn't create child @ " << pos << " for: " << path;
                  return false;
                }
              }
              curr = (Node*) curr->GetChildAt(pos);
            }
          }
        }
        return true;
      }

      static inline bool
      Remove(Node* node, const std::string& path) {

      }

      static inline Node*
      Search(Node* node, const std::string& path) {
        
      }
    public:
      EventTrie():
        root_(nullptr) {
      }
      virtual ~EventTrie() = default;

      inline Node* GetRoot() const {
        return root_;
      }

      friend std::ostream& operator<<(std::ostream& stream, const EventTrie& trie) {
        stream << "EventTrie(";
        stream << ")";
        return stream;
      }
    };

    class DefaultEventBus : public EventBus {
    public:
      DefaultEventBus() = default;
      ~DefaultEventBus() override = default;
      void Register(const char* event) override;
      void Deregister(const char* event) override;
      void Publish(const char* event) override;
    };

    static EventBus* instance = nullptr;

    EventBus* EventBus::GetInstance() {
      if(instance == nullptr)
        instance = new DefaultEventBus();
      return instance;
    }
  }
}
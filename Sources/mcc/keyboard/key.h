#ifndef MCC_KEY_CODE_H
#define MCC_KEY_CODE_H

#include <set>
#include <utility>

#include "mcc/rx.h"
#include "mcc/gfx.h"
#include "mcc/keyboard/key_state.h"

namespace mcc::keyboard {
  typedef int KeyCode;

  class Key {
    friend class KeySet;
  protected:
    KeyCode code_;

    KeyState GetState(GLFWwindow* window) const;

#define DEFINE_STATE_CHECK(Name, GlValue)                    \
    inline bool Is##Name(GLFWwindow* window) const {         \
      return GetState(window) == KeyState::kKey##Name;       \
    }
    FOR_EACH_KEY_STATE(DEFINE_STATE_CHECK)
#undef DEFINE_STATE_CHECK
  public:
    explicit Key(const KeyCode code):
      code_(code) {
    }
    Key():
      Key(GLFW_KEY_UNKNOWN) {
    }
    Key(const Key& rhs) = default;
    ~Key() = default;

    KeyCode GetCode() const {
      return code_;
    }

    std::string GetName() const {
      const auto name = glfwGetKeyName(GetCode(), 0);
      return name ? std::string(name) : std::string{};
    }

    bool IsUnknown() const {
      return GetCode() == GLFW_KEY_UNKNOWN;
    }

    Key& operator=(const Key& rhs) = default;
    
    bool operator<(const Key& rhs) const {
      return GetCode() < rhs.GetCode();
    }

    bool operator>(const Key& rhs) const {
      return GetCode() > rhs.GetCode();
    }

    bool operator==(const Key& rhs) const {
      return GetCode() == rhs.GetCode();
    }

    bool operator!=(const Key& rhs) const {
      return GetCode() != rhs.GetCode();
    }

    friend std::ostream& operator<<(std::ostream& stream, const Key& rhs) {
      stream << "Key(";
      stream << "name=" << rhs.GetName() << ", ";
      stream << "code=" << rhs.GetCode();
      stream << ")";
      return stream;
    }
  };
  
  class Keyboard;
  class KeySet {
  protected:
    const Keyboard* owner_;
    std::set<Key> keys_;

    inline const Keyboard*
    GetOwner() const {
      return owner_;
    }

    GLFWwindow* GetWindowHandle() const;

    inline void Insert(const KeyCode& code) {
      const auto result = keys_.insert(Key(code));
      DLOG_IF(WARNING, !result.second) << "failed to insert: " << code;
    }
  public:
    explicit KeySet(const Keyboard* owner);
    KeySet() = delete;
    KeySet(const KeySet& rhs) = delete;
    virtual ~KeySet() = default;

    KeyState GetState(const Key& key) const {
      return key.GetState(GetWindowHandle());
    }

    std::set<Key>::const_iterator begin() const {
      return keys_.begin();
    }

    std::set<Key>::const_iterator end() const {
      return keys_.end();
    }

    rx::observable<Key> GetAll() const {
      return rx::observable<>::iterate(keys_);
    }

    rx::observable<std::pair<Key, KeyState>> GetAllStates() const {
      return GetAll()
        .map([this](const Key& key) {
          return std::make_pair(key, key.GetState(GetWindowHandle()));
        });
    }

    KeySet& operator=(const KeySet& rhs) = delete;
    bool operator==(const KeySet& rhs) const = delete;
    bool operator!=(const KeySet& rhs) const = delete;
    bool operator<(const KeySet& rhs) const = delete;
    bool operator>(const KeySet& rhs) const = delete;
  };
}

#endif //MCC_KEY_CODE_H
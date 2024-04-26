#ifndef MCC_KEY_STATE_MAP_H
#define MCC_KEY_STATE_MAP_H

#include <map>
#include "mcc/keyboard/key.h"
#include "mcc/keyboard/key_state.h"

namespace mcc::keyboard {
  class KeyStateSet {
  protected:
    std::map<Key, KeyState> states_;
  public:
    explicit KeyStateSet(const KeySet& keys):
      states_() {
      keys.GetAllStates()
        .subscribe([this](const std::pair<Key, KeyState>& p) {
        });
    }
    virtual ~KeyStateSet() = default;

    std::optional<KeyState> GetState(const Key& k) {
      const auto pos = states_.find(k);
      if(pos == states_.end())
        return std::nullopt;
      return { pos->second };
    }

    inline std::optional<KeyState> GetState(const KeyCode code) {
      return GetState(Key(code));
    }

    void PutState(const std::pair<Key, KeyState>& p) {
      const auto result = states_.insert(p);
      if(!result.second)
        states_[p.first] = p.second;
    }

    inline void PutState(const Key& key, const KeyState state) {
      return PutState({ key, state });
    }

    inline void PutState(const KeyCode code, const KeyState state) {
      return PutState(Key(code), state);
    }
  };
}

#endif //MCC_KEY_STATE_MAP_H
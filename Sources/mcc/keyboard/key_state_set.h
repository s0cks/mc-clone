#ifndef MCC_KEY_STATE_MAP_H
#define MCC_KEY_STATE_MAP_H

#include "mcc/keyboard/keyboard_constants.h"

namespace mcc::keyboard {
  class KeyStateSet {
  protected:
    KeyState* keys_;
    uint32_t num_keys_;
  public:
    explicit KeyStateSet(const uint32_t num_keys = kNumberOfKeyCodes):
      keys_(nullptr),
      num_keys_(0) {
      if(num_keys <= 0) {
        DLOG(ERROR) << "cannot initialize KeyStateSet w/ " << num_keys << " keys.";
        return;
      }

      const auto data = malloc(sizeof(KeyState) * num_keys);
      if(!data) {
        LOG(ERROR) << "failed to initialize KeyStateSet w/ " << num_keys << " keys.";
        return;
      }
      keys_ = (KeyState*) data;
      num_keys_ = num_keys;
    }
    virtual ~KeyStateSet() {
      if(keys_)
        free(keys_);
    }

    virtual void Set(const KeyCode code, const KeyState state = kKeyPressed) {
      MCC_ASSERT(keys_);
      keys_[code] = state;
    }

    virtual KeyState Get(const KeyCode code) const {
      MCC_ASSERT(keys_);
      return keys_[code];
    }

    uint32_t length() const {
      return num_keys_;
    }
    
    inline bool Test(const KeyCode code) const {
      return Get(code) == kKeyPressed;
    }

    bool operator[](const KeyCode code) const {
      return Test(code);
    }
  };
}

#endif //MCC_KEY_STATE_MAP_H
#ifndef MCC_COMPONENT_STATE_H
#define MCC_COMPONENT_STATE_H

#include "mcc/platform.h"
#include "mcc/relaxed_atomic.h"

namespace mcc {
  template<typename C>
  class ComponentState {
  private:
    RelaxedAtomic<uword> data_;
  public:
    ComponentState() = default;
    ComponentState(const uword data):
      data_(data) {
    }
    ComponentState(C* data):
      ComponentState((uword) data) {
    }
    ComponentState(const C& data):
      ComponentState(new C(data)) {
    }
    ComponentState(const ComponentState& rhs):
      data_(rhs.data_) {
    }
    ~ComponentState() = default;

    uword raw_ptr() const {
      return (uword) data_;
    }

    C* data() const {
      return (C*) raw_ptr();
    }

    C* operator->() const {
      return data();
    }

    explicit operator uword () const {
      return raw_ptr();
    }

    explicit operator C*() const {
      return data();
    }

    void operator=(const ComponentState<C>& rhs) {
      data_ = rhs.data_;
    }

    void operator=(C* rhs) {
      data_ = (uword) rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ComponentState<C>& rhs) {
      stream << "ComponentState(";
      stream << "data=" << std::hex << rhs.data_;
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_COMPONENT_STATE_H
#ifndef MCC_EVENT_H
#define MCC_EVENT_H

#include <string>
#include "mcc/rx.h"

namespace mcc {
  class Event {
  protected:
    Event() = default;
  public:
    virtual ~Event() = default;
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const = 0;
  };
}

#endif //MCC_EVENT_H
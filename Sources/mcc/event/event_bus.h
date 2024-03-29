#ifndef MCC_EVENT_BUS_H
#define MCC_EVENT_BUS_H

#include <string>
#include <glog/logging.h>

#include "mcc/event/event.h"
#include "mcc/trie.h"

namespace mcc {
  namespace event {
    class EventBus {
    protected:
      EventBus() = default;
    public:
      virtual ~EventBus() = default;
      virtual void Register(const char* event) = 0;
      virtual void Deregister(const char* event) = 0; //DON'T USE
      virtual void Publish(const char* event) = 0;
    public:
      static EventBus* GetInstance();
    };
  }
}

#endif //MCC_EVENT_BUS_H
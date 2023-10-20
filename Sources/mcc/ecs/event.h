#ifndef MCC_EVENT_H
#define MCC_EVENT_H

#include <cstdint>
#include <any>
#include <list>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include "mcc/common.h"
#include "mcc/murmur.h"

namespace mcc {
  typedef uint64_t EventId;

  static inline EventId
  GetEventId(const char* value) {
    return Murmur(value, strlen(value));
  }

  class Event {
  protected:
    EventId id_;
    std::unordered_map<EventId, std::any> data_;
  public:
    Event() = delete;
    explicit Event(const EventId id):
      id_(id) {
    }
    virtual ~Event() = default;

    template<typename T>
    void SetParam(const EventId id, const T value) {
      data_[id] = value;
    }

    template<typename T>
    T GetParam(const EventId id) {
      return std::any_cast<T>(data_[id]);
    }

    EventId GetId() const {
      return id_;
    }
  };

  typedef std::function<void(Event&)> EventListener;

  class Events {
    DEFINE_NON_INSTANTIABLE_TYPE(Events);
  private:
  public:
    static void Register(const EventId event, EventListener listener);
    static void Send(Event& event);
    
    static inline void
    Send(const EventId event) {
      Event e(event);
      return Send(e);
    }
  };
}

#endif //MCC_EVENT_H
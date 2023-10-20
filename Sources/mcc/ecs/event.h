#ifndef MCC_EVENT_H
#define MCC_EVENT_H

#include <cstdint>
#include <any>
#include <list>
#include <algorithm>
#include <functional>
#include <unordered_map>

namespace mcc {
  typedef uint64_t EventId;

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

  class EventManager {
    friend class Coordinator;
  protected:
    std::unordered_map<EventId, std::list<EventListener>> listeners_;

    EventManager():
      listeners_() {
    }
  public:
    virtual ~EventManager() = default;

    void Register(const EventId event, EventListener listener) {
      listeners_[event].push_back(listener);
    }

    void Send(const EventId event) {
      Event e(event);
      return Send(e);
    }

    void Send(Event& event) {
      const auto listeners = listeners_[event.GetId()];
      std::for_each(listeners.begin(), listeners.end(), [&event](const EventListener& listener) {
        listener(event);
      });
    }
  };
}

#endif //MCC_EVENT_H
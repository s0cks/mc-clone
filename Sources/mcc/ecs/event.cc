#include "mcc/ecs/event.h"

namespace mcc {
  static std::unordered_map<EventId, std::list<EventListener>> listeners_;

  void Events::Register(const EventId event, EventListener listener) {
    listeners_[event].push_back(listener);
  }

  void Events::Send(Event& event) {
    const auto listeners = listeners_[event.GetId()];
    std::for_each(listeners.begin(), listeners.end(), [&event](const EventListener& listener) {
      listener(event);
    });
  }
}
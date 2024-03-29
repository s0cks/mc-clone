#ifndef MCC_EVENT_H
#define MCC_EVENT_H

namespace mcc {
  namespace event {
    class Event {
    protected:
      Event() = default;
    public:
      virtual ~Event() = default;
      virtual const char* GetName() const = 0;
    };
  }
}

#endif //MCC_EVENT_H
#include "mcc/event/test_event_bus.h"

namespace mcc::event {
  struct TestEvent {
    std::string message;
  };

  TEST_F(EventBusTest, TestEventBus) {
    EventBus bus;
  }
}
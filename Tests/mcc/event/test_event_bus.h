#ifndef MCC_TEST_EVENT_BUS_H
#define MCC_TEST_EVENT_BUS_H

#include <gtest/gtest.h>

#include "mcc/event/event_bus.h"

namespace mcc::event {
  class EventBusTest : public ::testing::Test {
  protected:
    EventBusTest() = default;
  public:
    ~EventBusTest() override = default;
  };
}

#endif //MCC_TEST_EVENT_BUS_H
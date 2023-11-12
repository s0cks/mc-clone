#ifndef MCC_TEST_COMPONENT_STATE_TABLE_H
#define MCC_TEST_COMPONENT_STATE_TABLE_H

#include <gtest/gtest.h>
#include "mcc/component/component_state_table.h"

namespace mcc {
  class ComponentStateTableTest : public ::testing::Test {
  protected:
    ComponentStateTableTest() = default;
  public:
    ~ComponentStateTableTest() override = default;
  };
}

#endif //MCC_TEST_COMPONENT_STATE_TABLE_H
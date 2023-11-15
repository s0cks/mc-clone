#ifndef MCC_TEST_RESOURCE_H
#define MCC_TEST_RESOURCE_H

#include <gtest/gtest.h>
#include "mcc/resource.h"

namespace mcc {
  class ResourceTest : public ::testing::Test {
  protected:
    ResourceTest() = default;
  public:
    ~ResourceTest() override = default;
  };
}

#endif //MCC_TEST_RESOURCE_H
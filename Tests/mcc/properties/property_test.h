#ifndef MCC_PROPERTY_TEST_H
#define MCC_PROPERTY_TEST_H

#include <gtest/gtest.h>
#include "mcc/properties/property.h"
#include "mcc/properties/property_assertions.h"

namespace mcc::properties {
  using namespace ::testing;

  class PropertyTest : public Test {
  protected:
    PropertyTest() = default;
  public:
    ~PropertyTest() override = default;
  };
}

#endif //MCC_PROPERTY_TEST_H
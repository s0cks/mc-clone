#ifndef MCC_TEST_MATERIAL_H
#define MCC_TEST_MATERIAL_H

#include <gtest/gtest.h>
#include "mcc/material/material.h"

namespace mcc {
  class MaterialTest : public ::testing::Test {
  protected:
    MaterialTest() = default;
  public:
    ~MaterialTest() override = default;
  };
}

#endif //MCC_TEST_MATERIAL_H
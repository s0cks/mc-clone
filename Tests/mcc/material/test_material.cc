#include <gtest/gtest.h>

#include "mcc/mock_gl.h"
#include "mcc/material/material.h"

namespace mcc::material {
  class MaterialTest : public ::testing::Test {
  protected:
    MaterialTest() = default;
    ~MaterialTest() override = default;
  };

  TEST_F(MaterialTest, Test_Bind_WillPass) {
    MockGl gl;

    
  }
}
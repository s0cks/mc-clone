#include <gtest/gtest.h>

#include "mcc/mock_gl.h"
#include "mcc/ibo/ibo_factory.h"

namespace mcc::ibo {
  using namespace ::testing;

  class IboFactoryTest : public Test {
  protected:
    IboFactoryTest() = default;
  public:
    ~IboFactoryTest() override = default;
  };

  TEST_F(IboFactoryTest, Test_Create_Will_Pass) {
    MockGl gl;
  }
}
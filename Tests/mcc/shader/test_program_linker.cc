#include <gtest/gtest.h>

#include "mcc/program/program_linker.h"

namespace mcc::shader {
  using namespace ::testing;

  class ProgramLinkerTest : public Test {
  protected:
    ProgramLinkerTest() = default;
  public:
    ~ProgramLinkerTest() override = default;
  };
}